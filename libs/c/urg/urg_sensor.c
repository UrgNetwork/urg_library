/*!
  \brief URG センサ制御

  URG 用の基本的な関数を提供します。

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"


void urg_initialize(urg_t *urg)
{
    (void)urg;
    // !!!
}


int urg_open(urg_t *urg, const char *device, long baudrate,
             connection_type_t connection_type)
{
    (void)urg;
    (void)device;
    (void)baudrate;
    (void)connection_type;

    // !!!

    return -1;
}


void urg_close(urg_t *urg)
{
    (void)urg;
    // !!!
}


int urg_start_timestamp_mode(urg_t *urg)
{
    (void)urg;
    // !!!
    return -1;
}


long urg_timestamp(urg_t *urg)
{
    (void)urg;
    // !!!
    return -1;
}


void urg_stop_timestamp_mode(urg_t *urg)
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


int urg_get_distance(urg_t *urg, long data[], long *timestamp)
{
    (void)urg;
    (void)data;
    (void)timestamp;

    // !!!

    return -1;
}


int urg_get_distance_intensity(urg_t *urg,
                               long data[], unsigned short intensity[],
                               long *timestamp)
{
    (void)urg;
    (void)data;
    (void)intensity;
    (void)timestamp;

    // !!!

    return -1;
}


int urg_get_multiecho(urg_t *urg, long data_multi[], long *timestamp)
{
    (void)urg;
    (void)data_multi;
    (void)timestamp;

    // !!!

    return -1;
}


int urg_get_multiecho_intensity(urg_t *urg,
                                long data_multi[],
                                unsigned short intensity_multi[],
                                long *timestamp)
{
    (void)urg;
    (void)data_multi;
    (void)intensity_multi;
    (void)timestamp;

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
