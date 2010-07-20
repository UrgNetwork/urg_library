/*!
  \brief URG センサ制御

  URG 用の基本的な関数を提供します。

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_errno.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


enum {
    URG_FALSE = 0,
    URG_TRUE = 1,

    BUFFER_SIZE = 64,

    EXPECTED_END = -1,

    RECEIVE_DATA_TIMEOUT,
    RECEIVE_DATA_COMPLETE,      /*!< データを正常に受信 */

    PP_RESPONSE_LINES = 10,
    VV_RESPONSE_LINES = 7,
    II_RESPONSE_LINES = 9,

    MAX_TIMEOUT = 120,
};


static const char NOT_CONNECTED_MESSAGE[] = "not connected.";
static const char RECEIVE_ERROR_MESSAGE[] = "receive error.";


// 受信した応答の行数を返す
static int scip_response(urg_t *urg, const char* command,
                         const int expected_ret[], int timeout,
                         char *receive_buffer, int receive_buffer_max_size)
{
    char *p = receive_buffer;
    char buffer[BUFFER_SIZE];
    int filled_size = 0;
    int line_number = 0;
    int ret_val = URG_INVALID_RESPONSE;

    int write_size = strlen(command);
    int n = connection_write(&urg->connection, command, write_size);
    if (n != write_size) {
        return URG_SEND_ERROR;
    }

    if (p) {
        *p = '\0';
    }

    do {
        // !!! receive_buffer に BUFEER_SIZE 以上の空きがあるときは、p に直接格納する
        n = connection_readline(&urg->connection,
                                buffer, BUFFER_SIZE, timeout);
        if (n < 0) {
            return URG_NO_RESPONSE;
        }

        // チェックサムの評価
        // !!!
        // scip_checksum(const char buffer[], int size)
        // !!! URG_CHECKSUM_ERROR

        if (line_number == 0) {
            fprintf(stderr, "%s, %s\n", buffer, command);
            // エコーバック文字列が、一致するかを確認する
            if (strncmp(buffer, command, write_size - 1)) {
                return URG_INVALID_RESPONSE;
            }
        } else if (p && (n > 0) &&
                   (n < (receive_buffer_max_size - filled_size))) {
            memcpy(p, buffer, n);
            p += n;
            *p++ = '\0';
            filled_size += n;
        }

        // ステータス応答を評価して、戻り値を決定する
        if (line_number == 1) {
            if (n == 1) {
                // SCIP 1.1 応答の場合は、正常応答とみなす
                ret_val = 0;

            } else if (n != 3) {
                // !!!
                return URG_INVALID_RESPONSE;

            } else {
                int i;
                int actual_ret = strtol(buffer, NULL, 10);
                for (i = 0; expected_ret[i] != EXPECTED_END; ++i) {
                    if (expected_ret[i] == actual_ret) {
                        ret_val = 0;
                        break;
                    }
                }
            }
        }

        ++line_number;
    } while (n > 0);

    return (ret_val < 0) ? ret_val : (line_number - 1);
}


static void ignore_receive_data(connection_t *connection, int timeout)
{
    char buffer[BUFFER_SIZE];
    int n;

    do {
        n = connection_readline(connection,
                                buffer, BUFFER_SIZE, timeout);
    } while (n >= 0);
}


// ボーレートを変更しながら接続する
static int connect_serial_device(urg_t *urg, long baudrate)
{
    long try_baudrate[] = { 19200, 38400, 115200 };
    int try_times = sizeof(try_baudrate) / sizeof(try_baudrate[0]);
    int i;

    // 指示されたボーレートから接続する
    for (i = 0; i < try_times; ++i) {
        if (try_baudrate[i] == baudrate) {
            try_baudrate[i] = try_baudrate[0];
            try_baudrate[0] = baudrate;
            break;
        }
    }

    for (i = 0; i < try_times; ++i) {
        connection_set_baudrate(&urg->connection, try_baudrate[i]);

        enum { RECEIVE_BUFFER_SIZE = 4 };
        int qt_expected[] = { 0, EXPECTED_END };
        char receive_buffer[RECEIVE_BUFFER_SIZE];

        // QT を送信し、応答が返されるかでボーレートが一致しているかを確認する
        int ret = scip_response(urg, "QT\n", qt_expected, MAX_TIMEOUT,
                                receive_buffer, RECEIVE_BUFFER_SIZE);

        if (!strcmp("E", receive_buffer)) {
            // "E" が返された場合は、SCIP 1.1 とみなし "SCIP2.0" を送信する
            int scip20_expected[] = { 0, EXPECTED_END };
            ret = scip_response(urg, "SCIP2.0\n", scip20_expected,
                                MAX_TIMEOUT, NULL, 0);
            // !!!

            // ボーレートを変更して戻る
            // !!!
            return 0;
        }

        if (ret <= 0) {
            fprintf(stderr, "ret = %d\n", ret);
            if (ret == URG_INVALID_RESPONSE) {
                // 異常なエコーバックのときは、距離データ受信中とみなして
                // データを読み飛ばす
                ignore_receive_data(&urg->connection, MAX_TIMEOUT);

                // ボーレートを変更して戻る
                // !!!
                return 0;

            } else {
                // 応答がないときは、ボーレートを変更して、再度接続を行う
                continue;
            }
        } else if (!strcmp("00P", receive_buffer)) {
            // センサとホストのボーレートを変更して戻る
            // !!!
            return 0;
        }
    }

    urg->last_errno = URG_NOT_DETECT_BAUDRATE_ERROR;
    return urg->last_errno;
}


// PP コマンドの応答を urg_t に格納する
static int receive_parameter(urg_t *urg)
{
    enum { RECEIVE_BUFFER_SIZE = BUFFER_SIZE * 9, };
    char receive_buffer[RECEIVE_BUFFER_SIZE];
    int pp_expected[] = { 0, EXPECTED_END };
    unsigned short received_bits = 0x0000;
    char *p;
    int i;

    int ret = scip_response(urg, "PP\n", pp_expected, MAX_TIMEOUT,
                            receive_buffer, RECEIVE_BUFFER_SIZE);
    if (ret < PP_RESPONSE_LINES) {
        ignore_receive_data(&urg->connection, MAX_TIMEOUT);
        return URG_INVALID_RESPONSE;
    }

    p = receive_buffer;
    for (i = 0; i < (ret - 1); ++i) {

        if (!strncmp(p, "DMIN:", 5)) {
            urg->min_distance = strtol(p + 5, NULL, 10);
            received_bits |= 0x0001;

        } else if (!strncmp(p, "DMAX:", 5)) {
            urg->max_distance = strtol(p + 5, NULL, 10);
            received_bits |= 0x0002;

        } else if (!strncmp(p, "ARES:", 5)) {
            urg->area_resolution = strtol(p + 5, NULL, 10);
            received_bits |= 0x0004;

        } else if (!strncmp(p, "AMIN:", 5)) {
            urg->first_data_index = strtol(p + 5, NULL, 10);
            received_bits |= 0x0008;

        } else if (!strncmp(p, "AMAX:", 5)) {
            urg->last_data_index = strtol(p + 5, NULL, 10);
            received_bits |= 0x0010;

        } else if (!strncmp(p, "AFRT:", 5)) {
            urg->front_data_index = strtol(p + 5, NULL, 10);
            received_bits |= 0x0020;

        } else if (!strncmp(p, "SCAN:", 5)) {
            int rpm = strtol(p + 5, NULL, 10);
            urg->scan_usec = 1000 * 1000 * 60 / rpm;
            urg->timeout = urg->scan_usec >> (10 - 1);
            received_bits |= 0x0040;
        }
        p += strlen(p) + 1;
    }

    // 全てのパラメータを受信したか確認
    if (received_bits != 0x007f) {
        return URG_RECEIVE_ERROR;
    }

    urg_set_scanning_parameter(urg,
                               urg->first_data_index - urg->front_data_index,
                               urg->last_data_index - urg->front_data_index,
                               1);

    return URG_NO_ERROR;
}


//! チェックサムの計算
static char scip_checksum(const char buffer[], int size)
{
    unsigned char sum = 0x00;
    int i;

    for (i = 0; i < size; ++i) {
        sum += buffer[i];
    }

    // 計算の意味は SCIP 仕様書を参照のこと
    return (sum & 0x3f) + 0x30;
}


//! SCIP 文字列のデコード
static long scip_decode(const char data[], int size)
{
    const char* p = data;
    const char* last_p = p + size;
    int value = 0;

    while (p < last_p) {
        value <<= 6;
        value &= ~0x3f;
        value |= *p++ - 0x30;
    }
    return value;
}


measurement_type_t parse_distance_echoback(urg_t *urg,
                                           const char echoback_line[])
{
    int line_length;
    measurement_type_t ret_type = URG_UNKNOWN;

    if (!strcmp("QT", echoback_line)) {
        return URG_STOP;
    }

    line_length = strlen(echoback_line);
    if ((line_length == 12) &&
        ((echoback_line[0] == 'G') || (echoback_line[0] == 'M')) &&
        ((echoback_line[1] == 'S') || (echoback_line[1] == 'D'))) {
        // !!!

    } else if ((line_length == 12) &&
               ((echoback_line[0] == 'G') || (echoback_line[0] == 'M')) &&
               (echoback_line[1] == 'E')) {
        // !!!

    } else if ((line_length == 15) &&
               ((echoback_line[0] == 'H') || (echoback_line[0] == 'N')) &&
               (echoback_line[1] == 'D')) {
        // !!!

    } else if ((line_length == 15) &&
               ((echoback_line[0] == 'H') || (echoback_line[0] == 'N')) &&
               (echoback_line[1] == 'E')) {
        // !!!
    }

    (void)urg;

    // !!!

    // !!!
    return ret_type;
}


static int receive_data_line(urg_t *urg, long data[],
                             unsigned short intensity[]) {
    (void)urg;
    (void)data;
    (void)intensity;
    // !!!

    return -1;
}

//! 距離データの取得
static int receive_data(urg_t *urg, long data[], unsigned short intensity[],
                        long *time_stamp)
{
    measurement_type_t type;
    int ret_code;
    char buffer[BUFFER_SIZE];
    int ret;
    int n;

    // エコーバックの取得
    n = connection_readline(&urg->connection,
                            buffer, BUFFER_SIZE, urg->timeout);
    // !!! チェックサムの確認
    fprintf(stderr, "n = %d\n", n);
    if (n > 0) {
        fprintf(stderr, "%s\n", buffer);
    }

    if (n <= 0) {
        return URG_NO_RESPONSE;
    }
    // エコーバックの解析
    type = parse_distance_echoback(urg, buffer);

    // 応答の取得
    n = connection_readline(&urg->connection,
                            buffer, BUFFER_SIZE, urg->timeout);
    // !!! チェックサムの確認
    if (n != 3) {
        ignore_receive_data(&urg->connection, urg->timeout);
        return URG_INVALID_RESPONSE;
    }
    ret_code = scip_decode(buffer, 2);

    fprintf(stderr, "n2 = %d, %d\n", n, ret_code);
    if (n > 0) {
        fprintf(stderr, "%s\n", buffer);
    }
    fprintf(stderr, "specified_scan_times: %d\n", urg->specified_scan_times);

    if (urg->specified_scan_times != 1) {
        if (ret_code == 0) {
            // 最後の空行を読み捨て、次からのデータを返す
            n = connection_readline(&urg->connection,
                                    buffer, BUFFER_SIZE, urg->timeout);
            if (n != 0) {
                ignore_receive_data(&urg->connection, urg->timeout);
                return URG_INVALID_RESPONSE;
            } else {
                return receive_data(urg, data, intensity, time_stamp);
            }
        }
    }

    if (((urg->specified_scan_times == 1) && (ret_code != 0)) ||
        ((urg->specified_scan_times != 1) && (ret_code != 99))) {
        // Gx, Hx のときは 00P が返されたときがデータ
        // Mx, Nx のときは 99b が返されたときがデータ
        ignore_receive_data(&urg->connection, urg->timeout);
        return URG_INVALID_RESPONSE;
    }

    // タイムスタンプの取得
    n = connection_readline(&urg->connection,
                            buffer, BUFFER_SIZE, urg->timeout);
    fprintf(stderr, "n3 = %d\n", n);
    if (n > 0) {
        if (time_stamp) {
            *time_stamp = scip_decode(buffer, 4);
            fprintf(stderr, "timestamp: %ld\n", *time_stamp);
        }
        fprintf(stderr, "%s\n", buffer);
    }

    // データの取得
    switch (type) {
    case URG_DISTANCE:
        ret = receive_data_line(urg, data, NULL);
        break;

    case URG_DISTANCE_INTENSITY:
        // !!!
        (void)data;
        (void)intensity;
        ret = 0;
        break;

    case URG_MULTIECHO:
        // !!!
        (void)data;
        ret = 0;
        break;

    case URG_MULTIECHO_INTENSITY:
        // !!!
        (void)data;
        (void)intensity;
        ret = 0;
        break;

    case URG_STOP:
    case URG_UNKNOWN:
        ret = 0;
        break;
    }

    if ((urg->specified_scan_times > 0) && (urg->scanning_remain_times > 0)) {
        if (--urg->scanning_remain_times <= 0) {
            urg_laser_off(urg);
        }
    }
    return ret;
}


int urg_open(urg_t *urg, connection_type_t connection_type,
             const char *device, long baudrate)
{
    int ret;

    urg->is_active = URG_FALSE;

    // デバイスへの接続
    if (connection_open(&urg->connection, connection_type,
                        device, baudrate) < 0) {
        switch (connection_type) {
        case URG_SERIAL:
            urg->last_errno = URG_SERIAL_OPEN_ERROR;
            break;
        case URG_ETHERNET:
            urg->last_errno = URG_ETHERNET_OPEN_ERROR;
        }
        return urg->last_errno;
    }

    // 指定したボーレートで URG と通信できるように調整
    if (connection_type == URG_SERIAL) {
        ret = connect_serial_device(urg, baudrate);
        if (ret != URG_NO_ERROR) {
            return ret;
        }
    }

    // 変数の初期化
    urg->last_errno = URG_NO_ERROR;
    urg->range_data_byte = URG_COMMUNICATION_3_BYTE;
    urg->specified_scan_times = 0;
    urg->scanning_remain_times = 0;
    urg->is_laser_on = URG_FALSE;

    // パラメータ情報を取得
    ret = receive_parameter(urg);
    if (ret == URG_NO_ERROR) {
        urg->is_active = URG_TRUE;
    }
    return ret;
}


void urg_close(urg_t *urg)
{
    connection_close(&urg->connection);
    urg->is_active = URG_FALSE;
}


int urg_start_time_stamp_mode(urg_t *urg)
{
    const int expected[] = { 0, EXPECTED_END };

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // TM0 を発行する
    return scip_response(urg, "TM0\n", expected, urg->timeout, NULL, 0);
}


long urg_time_stamp(urg_t *urg)
{
    const int expected[] = { 0, EXPECTED_END };
    char buffer[BUFFER_SIZE];
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    ret = scip_response(urg, "TM1\n", expected,
                        urg->timeout, buffer, BUFFER_SIZE);
    if (ret < 0) {
        return ret;
    }

    // buffer からタイムスタンプを取得し、デコードして返す
    if (strlen(buffer) != 5) {
        return URG_RECEIVE_ERROR;
    }
    if (buffer[5] == scip_checksum(buffer, 4)) {
        return URG_CHECKSUM_ERROR;
    }
    return scip_decode(buffer, 4);
}


void urg_stop_time_stamp_mode(urg_t *urg)
{
    int expected[] = { 0, EXPECTED_END };

    if (!urg->is_active) {
        return;
    }

    // TM2 を発行する
    scip_response(urg, "TM2\n", expected, urg->timeout, NULL, 0);
}


static int send_distance_command(urg_t *urg, int scan_times, int skip_scan)
{
    // !!! データを返す回数を指定することにして、
    // !!! actual_scan_times には 0 を返す

    char buffer[BUFFER_SIZE];
    char range_byte_ch =
        (urg->range_data_byte == URG_COMMUNICATION_2_BYTE) ? 'S' : 'D';
    int write_size = 0;
    int front_index = urg->front_data_index;
    int n;

    urg->specified_scan_times = (scan_times < 0) ? 0 : scan_times;
    urg->scanning_remain_times = urg->specified_scan_times;
    if (urg->scanning_remain_times == 1) {

        // レーザ発光を指示
        urg_laser_on(urg);

        // GD, GS
        write_size = snprintf(buffer, BUFFER_SIZE, "G%c%04d%04d%02d\n",
                              range_byte_ch,
                              urg->scanning_first_step + front_index,
                              urg->scanning_last_step + front_index,
                              urg->scanning_skip_step);
    } else {
        // MD, MS
        write_size = snprintf(buffer, BUFFER_SIZE, "M%c%04d%04d%02d%01d%02d\n",
                              range_byte_ch,
                              urg->scanning_first_step + front_index,
                              urg->scanning_last_step + front_index,
                              urg->scanning_skip_step,
                              skip_scan, 0);
    }

    n = connection_write(&urg->connection, buffer, write_size);
    if (n != 3) {
        return URG_SEND_ERROR;
    }
    return 0;
}


static int send_distance_intensity_command(urg_t *urg,
                                           int scan_times, int skip_scan)
{
    (void)urg;
    (void)scan_times;
    (void)skip_scan;
    // !!!
    return -1;
}


static int send_multiecho_command(urg_t *urg,
                                  int scan_times, int skip_scan)
{
    (void)urg;
    (void)scan_times;
    (void)skip_scan;
    // !!!
    return -1;
}


static int send_multiecho_intensity_command(urg_t *urg,
                                            int scan_times, int skip_scan)
{
    (void)urg;
    (void)scan_times;
    (void)skip_scan;
    // !!!
    return -1;
}


int urg_start_measurement(urg_t *urg, measurement_type_t type,
                          int scan_times, int skip_scan)
{
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    if ((skip_scan < 0) || (skip_scan > 9)) {
        ignore_receive_data(&urg->connection, urg->timeout);
        return URG_INVALID_PARAMETER;
    }

    // 指定されたタイプのパケットを生成し、送信する
    switch (type) {
    case URG_DISTANCE:
        ret = send_distance_command(urg, scan_times, skip_scan);
        break;

    case URG_DISTANCE_INTENSITY:
        ret = send_distance_intensity_command(urg, scan_times, skip_scan);
        break;

    case URG_MULTIECHO:
        ret = send_multiecho_command(urg, scan_times, skip_scan);
        break;

    case URG_MULTIECHO_INTENSITY:
        ret = send_multiecho_intensity_command(urg, scan_times, skip_scan);
        break;

    case URG_STOP:
    case URG_UNKNOWN:
        ignore_receive_data(&urg->connection, urg->timeout);
        ret = URG_INVALID_PARAMETER;
        break;
    }

    return ret;
}


int urg_get_distance(urg_t *urg, long data[], long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }
    return receive_data(urg, data, NULL, time_stamp);
}


int urg_get_distance_intensity(urg_t *urg,
                               long data[], unsigned short intensity[],
                               long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    return receive_data(urg, data, intensity, time_stamp);
}


int urg_get_multiecho(urg_t *urg, long data_multi[], long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    return receive_data(urg, data_multi, NULL, time_stamp);
}


int urg_get_multiecho_intensity(urg_t *urg,
                                long data_multi[],
                                unsigned short intensity_multi[],
                                long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    return receive_data(urg, data_multi, intensity_multi, time_stamp);
}


int urg_stop_measurement(urg_t *urg)
{
    enum { MAX_READ_TIMES = 3 };
    int ret = URG_INVALID_RESPONSE;
    int n;
    int i;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // QT を発行する
    n = connection_write(&urg->connection, "QT\n", 3);
    if (n != 3) {
        return URG_SEND_ERROR;
    }

    for (i = 0; i < MAX_READ_TIMES; ++i) {
        // QT の応答が返されるまで、距離データを読み捨てる
        ret = receive_data(urg, NULL, NULL, NULL);
        if (ret == URG_STOP) {
            // 正常応答
            ret = 0;
            break;
        }
    }
    return ret;
}


int urg_set_scanning_parameter(urg_t *urg, int first_step, int last_step,
                               int skip_step)
{
    // 設定の範囲外を指定したときは、エラーを返す
    if (((skip_step < 0) || (skip_step >= 100)) ||
        (first_step > last_step) ||
        (first_step < -urg->front_data_index) ||
        (last_step > (urg->last_data_index - urg->front_data_index))) {
        return URG_SCANNING_PARAMETER_ERROR;
    }

    urg->scanning_first_step = first_step;
    urg->scanning_last_step = last_step;
    urg->scanning_skip_step = skip_step;

    return 0;
}


int urg_set_connection_data_size(urg_t *urg,
                                 range_data_byte_t range_data_byte)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    if ((range_data_byte != URG_COMMUNICATION_3_BYTE) ||
        (range_data_byte != URG_COMMUNICATION_2_BYTE)) {
        return URG_DATA_SIZE_PARAMETER_ERROR;
    }

    urg->range_data_byte = range_data_byte;

    return 0;
}


int urg_laser_on(urg_t *urg)
{
    int expected[] = { 0, 2, EXPECTED_END };
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    if (urg->is_laser_on != URG_FALSE) {
        // 既にレーザが発光しているときは、コマンドを送信しないようにする
        return 0;
    }

    ret = scip_response(urg, "BM\n", expected, urg->timeout, NULL, 0);
    if (ret == URG_NO_ERROR) {
        urg->is_laser_on = URG_TRUE;
    }
    return ret;
}


int urg_laser_off(urg_t *urg)
{
    return urg_stop_measurement(urg);
}


int urg_reboot(urg_t *urg)
{
    (void)urg;
    // !!!

    // RB コマンドを２回送信する
    // !!!

    // ２回目の RB 送信後、１秒以内に接続を切断する
    // !!!

    return -1;
}


static char *copy_token(char *dest, char *receive_buffer,
                        const char *start_str, char end_ch, int lines)
{
    char *p = receive_buffer;
    int start_str_len = strlen(start_str);
    int i;

    for (i = 0; i < lines; ++i) {
        if (!strncmp(p, start_str, start_str_len)) {

            char *last_p = strchr(p + start_str_len, end_ch);
            if (last_p) {
                *last_p = '\0';
                memcpy(dest, p + start_str_len, last_p - (p + start_str_len));
                return dest;
            }
        }
        p += strlen(p) + 1;
    }
    return NULL;
}


const char *urg_sensor_id(urg_t *urg)
{
    enum { RECEIVE_BUFFER_SIZE = BUFFER_SIZE * 4, };
    char receive_buffer[RECEIVE_BUFFER_SIZE];
    int vv_expected[] = { 0, EXPECTED_END };
    int ret;
    char *p;

    if (!urg->is_active) {
        return NOT_CONNECTED_MESSAGE;
    }

    ret = scip_response(urg, "VV\n", vv_expected, urg->timeout,
                        receive_buffer, RECEIVE_BUFFER_SIZE);
    if (ret < VV_RESPONSE_LINES) {
        return RECEIVE_ERROR_MESSAGE;
    }

    p = copy_token(urg->return_buffer, receive_buffer, "SERI:", ';', ret - 1);
    return (p) ? p : RECEIVE_ERROR_MESSAGE;
}


const char *urg_sensor_version(urg_t *urg)
{
    enum { RECEIVE_BUFFER_SIZE = BUFFER_SIZE * 4, };
    char receive_buffer[RECEIVE_BUFFER_SIZE];
    int vv_expected[] = { 0, EXPECTED_END };
    int ret;
    char *p;

    if (!urg->is_active) {
        return NOT_CONNECTED_MESSAGE;
    }

    ret = scip_response(urg, "VV\n", vv_expected, urg->timeout,
                        receive_buffer, RECEIVE_BUFFER_SIZE);
    if (ret < VV_RESPONSE_LINES) {
        return RECEIVE_ERROR_MESSAGE;
    }

    p = copy_token(urg->return_buffer, receive_buffer, "FIRM:", '(', ret - 1);
    return (p) ? p : RECEIVE_ERROR_MESSAGE;
}


const char *urg_sensor_status(urg_t *urg)
{
    enum { RECEIVE_BUFFER_SIZE = BUFFER_SIZE * 4, };
    char receive_buffer[RECEIVE_BUFFER_SIZE];
    int ii_expected[] = { 0, EXPECTED_END };
    int ret;
    char *p;

    if (!urg->is_active) {
        return NOT_CONNECTED_MESSAGE;
    }

    ret = scip_response(urg, "II\n", ii_expected, urg->timeout,
                        receive_buffer, RECEIVE_BUFFER_SIZE);
    if (ret < II_RESPONSE_LINES) {
        return RECEIVE_ERROR_MESSAGE;
    }

    p = copy_token(urg->return_buffer, receive_buffer, "STAT:", ';', ret - 1);
    return (p) ? p : RECEIVE_ERROR_MESSAGE;
}


int urg_find_port(char *port_name, int index)
{
    (void)port_name;
    (void)index;

    // !!!

    // !!! ETHERNET のときは、エラーメッセージを表示する

    return 0;
}

