/*!
  \brief URG センサ制御

  URG 用の基本的な関数を提供します。

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_errno.h"

enum {
    URG_FALSE = 0,
    URG_TRUE = 1,
};


int urg_open(urg_t *urg, connection_type_t connection_type,
             const char *device, long baudrate)
{
    urg->is_active = URG_FALSE;

    if (! connection_open(&urg->connection, connection_type,
                          device, baudrate)) {

        // !!!
        return URG_OPEN_ERROR;
    }

    (void)urg;
    (void)device;
    (void)baudrate;
    (void)connection_type;

    // !!!

    return -1;
}


void urg_close(urg_t *urg)
{
    // !!! serial_close() を呼ぶ

    // !!! 構造体をクリアする
    // !!!

    (void)urg;
    // !!!
}


int urg_start_time_stamp_mode(urg_t *urg)
{
    // TM0 を発行する
    // !!

    (void)urg;
    // !!!
    return -1;
}


long urg_time_stamp(urg_t *urg)
{
    (void)urg;
    // !!!
    return -1;
}


void urg_stop_time_stamp_mode(urg_t *urg)
{
    (void)urg;
    // !!!
}


void urg_start_measurement(urg_t *urg, measurement_type_t type,
                           int scan_times, int skip_scan)
{
    (void)urg;
    (void)type;
    (void)scan_times;
    (void)skip_scan;
    // !!!
}


int urg_get_distance(urg_t *urg, long data[], long *time_stamp)
{
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
    (void)urg;
    (void)data;
    (void)intensity;
    (void)time_stamp;

    // !!!

    return -1;
}


int urg_get_multiecho(urg_t *urg, long data_multi[], long *time_stamp)
{
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
    (void)urg;
    (void)data_multi;
    (void)intensity_multi;
    (void)time_stamp;

    // !!!

    return -1;
}


void urg_stop_measurement(urg_t *urg)
{
    (void)urg;

    // !!!
}


int urg_set_scanning_parameter(urg_t *urg, int first_step, int last_step,
                               int skip_step)
{
    (void)urg;
    (void)first_step;
    (void)last_step;
    (void)skip_step;

    // !!!

    return -1;
}


int urg_set_communication_data_size(urg_t *urg, range_byte_t data_size)
{
    (void)urg;
    (void)data_size;

    // !!!

    return -1;
}


int urg_laser_on(urg_t *urg)
{
    (void)urg;
    // !!!
    return -1;
}


void urg_laser_off(urg_t *urg)
{
    (void)urg;
    // !!!
}
