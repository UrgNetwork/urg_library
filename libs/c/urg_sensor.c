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
#include <stdio.h>


enum {
    URG_FALSE = 0,
    URG_TRUE = 1,

    BUFFER_SIZE = 64,

    EXPECTED_END = -1,

    RECEIVE_DATA_TIMEOUT,
    RECEIVE_DATA_COMPLETE,      /*!< データを正常に受信 */
    RECEIVE_DATA_QT,            /*!< QT 応答を受信 */

    MAX_TIMEOUT = 120,
};


static int scip_response(urg_t *urg, const char* command,
                         const int expected_ret[], int timeout,
                         char *receive_buffer, int receive_buffer_max_size)
{
    (void)expected_ret;
    (void)receive_buffer;
    (void)receive_buffer_max_size;
    // !!!

    char *p = receive_buffer;
    char buffer[BUFFER_SIZE];
    int filled_size = 0;

    int write_size = strlen(command);
    int n = connection_write(&urg->connection, command, write_size);
    if (n != write_size) {
        // !!!
        return -1;
    }

    // !!! receive_buffer に BUFEER_SIZE 以上の空きがあるときは、直接格納する

    n = connection_readline(&urg->connection,
                            buffer, BUFFER_SIZE, timeout);
    fprintf(stderr, "n = %d\n", n);

    if (p && (n > receive_buffer_max_size - filled_size)) {
        memcpy(p, buffer, n);
        p += n;
        *p++ = '\0';
    }

    return -1;
}


// !!! ボーレートを変更しながら接続する
static int connect_serial_device(urg_t *urg, long baudrate)
{
    (void)urg;
    int i;

    long try_baudrate[] = { 19200, 38400, 115200 };
    int try_times = sizeof(try_baudrate) / sizeof(try_baudrate[0]);

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
        fprintf(stderr, "%d: %s\n", strlen(receive_buffer), receive_buffer);
        if (ret != 0) {
            if (1) {
                // "E" が返された場合は、SCIP 1.1 とみなし "SCIP2.0" を送信する
                // !!!
            } else if (0) {
                // データを受信した場合は、距離データ受信中とみなし、データを読み飛ばす
                // !!!
            } else {
                // ボーレートを変更して、再度接続を行う
                continue;
            }
        } else {
            // 正常に接続
            // !!!
        }

        // !!! SS

        if (0) {
            return 0;
        }
    }

    urg->last_errno = URG_NOT_DETECT_BAUDRATE_ERROR;
    return urg->last_errno;
}


// PP コマンドの応答を urg_t に格納する
static int receive_parameter(urg_t *urg)
{
    (void)urg;
    // !!!

    urg->timeout = 100;

    return -1;
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
static long scip_decode(const char buffer[], int size)
{
    (void)buffer;
    (void)size;
    // !!!

    return -1;
}


//! 距離データの取得
static int receive_data(urg_t *urg, long data[], unsigned short intensity[],
                        long *time_stamp)
{
    (void)urg;
    (void)data;
    (void)intensity;
    (void)time_stamp;

    // !!! 戻り値
    // !!! - エラー
    // !!! - 受信したデータ数
    // !!! - QT, RS, RT の応答を受信した

    // !!! urg->scanning_remain_times をデクリメントするか
    // !!! SCIP 中の応答から残り回数を取得する
    // !!! どちらを使うかは specified_scan_times を利用する

    return -1;
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
        if (ret < 0) {
            fprintf(stderr, "ret = %d\n", ret);
            return ret;
        }
    }

    // 変数の初期化
    urg->last_errno = URG_NO_ERROR;
    urg->range_data_byte = URG_COMMUNICATION_3_BYTE;
    urg->specified_scan_times = 0;
    urg->scanning_remain_times = 0;

    // パラメータ情報を取得
    return receive_parameter(urg);
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
    char buffer[BUFFER_SIZE];
    int actual_scan_times = (scan_times < 0) ? 0 : scan_times;
    char range_byte_ch =
        (urg->range_data_byte == URG_COMMUNICATION_2_BYTE) ? 'S' : 'D';
    int write_size = 0;

    urg->scanning_remain_times = scan_times;
    if (actual_scan_times == 1) {
        // GD, GS
        write_size = snprintf(buffer, BUFFER_SIZE, "G%c%04d%04d%02d\n",
                              range_byte_ch,
                              urg->scanning_first_step, urg->scanning_last_step,
                              urg->scanning_skip_step);
    } else {
        // MD, MS
        write_size = snprintf(buffer, BUFFER_SIZE, "M%c%04d%04d%02d%01d%02d\n",
                              range_byte_ch,
                              urg->scanning_first_step, urg->scanning_last_step,
                              urg->scanning_skip_step,
                              skip_scan, scan_times);
    }
    return connection_write(&urg->connection, buffer, write_size);
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
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    if ((skip_scan < 0) || (skip_scan > 9)) {
        return URG_INVALID_PARAMETER;
    }

    // 指定されたタイプのパケットを生成し、送信する
    switch (type) {
    case URG_DISTANCE:
        send_distance_command(urg, scan_times, skip_scan);
        break;

    case URG_DISTANCE_INTENSITY:
        send_distance_intensity_command(urg, scan_times, skip_scan);
        break;

    case URG_MULTIECHO:
        send_multiecho_command(urg, scan_times, skip_scan);
        break;

    case URG_MULTIECHO_INTENSITY:
        send_multiecho_intensity_command(urg, scan_times, skip_scan);
        break;
    }

    return -1;
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
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // QT を発行する
    connection_write(&urg->connection, "QT\n", 3);
    do {
        // QT の応答が返されるまで、距離データを読み捨てる
        ret = receive_data(urg, NULL, NULL, NULL);
        if (ret == RECEIVE_DATA_QT) {
            // 正常応答
            return 0;
        }
    } while (ret != RECEIVE_DATA_TIMEOUT);

    return ret;
}


int urg_set_scanning_parameter(urg_t *urg, int first_step, int last_step,
                               int skip_step)
{
    // 設定の範囲外を指定したときは、エラーを返す
    if (((skip_step < 0) || (skip_step >= 100)) ||
        (first_step > last_step) ||
        (first_step < urg->scanning_first_step) ||
        (last_step > urg->scanning_last_step)) {
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

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // 既にレーザが発光しているときは、コマンドを送信しないようにする
    // !!!

    return scip_response(urg, "BM\n", expected, urg->timeout, NULL, 0);
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
