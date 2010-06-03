/*!
  \brief URG センサ制御

  URG 用の基本的な関数を提供します。

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_errno.h"
#include <stddef.h>

enum {
    URG_FALSE = 0,
    URG_TRUE = 1,

    BUFFER_SIZE = 64,

    EXPECTED_END = -1,
};


// !!! ボーレートを変更しながら接続を行う
static int connect_serial_device(urg_t *urg, long baudrate)
{
    (void)urg;
    (void)baudrate;
    // !!!

    return -1;
}


static int receive_parameter(urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}


static int response(urg_t *urg, const char* command, int command_size,
                    const int expected_ret[],
                    char receive_buffer[], int receive_buffer_max_size)
{
    (void)urg;
    (void)command;
    (void)command_size;
    (void)expected_ret;
    (void)receive_buffer;
    (void)receive_buffer_max_size;
    // !!!

    return -1;
}


//! SCIP 文字列のデコード
static long decode(const char buffer[], int size)
{
    (void)buffer;
    (void)size;
    // !!!

    return -1;
}


int urg_open(urg_t *urg, connection_type_t connection_type,
             const char *device, long baudrate)
{
    int ret;

    urg->is_active = URG_FALSE;

    // デバイスへの接続
    if (! connection_open(&urg->connection, connection_type,
                          device, baudrate)) {
        switch (connection_type) {
        case URG_SERIAL:
            return URG_SERIAL_OPEN_ERROR;
        case URG_ETHERNET:
            return URG_ETHERNET_OPEN_ERROR;
        }
    }

    // 指定したボーレートで URG と通信できるように調整
    if (connection_type == URG_SERIAL) {
        ret = connect_serial_device(urg, baudrate);
        if (ret < 0) {
            return ret;
        }
    }

    // パラメータ情報を取得
    ret = receive_parameter(urg);
    if (ret < 0) {
        return ret;
    }

    return 0;
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
    return response(urg, "TM0\n", 4, expected, NULL, 0);
}


long urg_time_stamp(urg_t *urg)
{
    const int expected[] = { 0, EXPECTED_END };
    char buffer[BUFFER_SIZE];
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    ret = response(urg, "TM1\n", 4, expected, buffer, BUFFER_SIZE);
    if (ret < 0) {
        return ret;
    }

    // buffer からタイムスタンプを取得し、デコードして返す
    // !!!
    // !!! チェックサムの確認
    return decode(buffer, 4);
}


void urg_stop_time_stamp_mode(urg_t *urg)
{
    int expected[] = { 0, -1, };

    if (!urg->is_active) {
        return;
    }

    // TM2 を発行する
    response(urg, "TM2\n", 4, expected, NULL, 0);
}


int urg_start_measurement(urg_t *urg, measurement_type_t type,
                          int scan_times, int skip_scan)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)type;
    (void)scan_times;
    (void)skip_scan;
    // !!!

    return -1;
}


int urg_get_distance(urg_t *urg, long data[], long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)data;
    (void)time_stamp;

    // !!!

    return -1;
}


int urg_get_distance_intensity(urg_t *urg,
                               long data[], unsigned short intensity[],
                               long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)data;
    (void)intensity;
    (void)time_stamp;

    // !!!

    return -1;
}


int urg_get_multiecho(urg_t *urg, long data_multi[], long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)data_multi;
    (void)time_stamp;

    // !!!

    return -1;
}


int urg_get_multiecho_intensity(urg_t *urg,
                                long data_multi[],
                                unsigned short intensity_multi[],
                                long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)data_multi;
    (void)intensity_multi;
    (void)time_stamp;

    // !!!

    return -1;
}


void urg_stop_measurement(urg_t *urg)
{
    if (!urg->is_active) {
        return;
    }

    (void)urg;

    // !!!
}


int urg_set_scanning_parameter(urg_t *urg, int first_step, int last_step,
                               int skip_step)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)first_step;
    (void)last_step;
    (void)skip_step;

    // !!!

    return -1;
}


int urg_set_communication_data_size(urg_t *urg, range_byte_t data_size)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)data_size;

    // !!!

    return -1;
}


int urg_laser_on(urg_t *urg)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    // !!!
    return -1;
}


int urg_laser_off(urg_t *urg)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    // !!!

    return -1;
}
