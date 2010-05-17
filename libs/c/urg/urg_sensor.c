/*!
  \file
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


long urg_get_timestamp(urg_t *urg)
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


int urg_get_distance(urg_t *urg, long data[], long *timestamp)
{
    (void)urg;
    (void)data;
    (void)timestamp;
    // !!!
    return -1;
}


int urg_start_measurement(urg_t *urg, int scan_times, int skip_scan)
{
    (void)urg;
    (void)scan_times;
    (void)skip_scan;
    // !!!
    return -1;
}


int urg_receive_measurement(urg_t *urg, long data[], long *timestamp)
{
    (void)urg;
    (void)data;
    (void)timestamp;
    // !!!
    return -1;
}


int urg_start_intensity_measurement(urg_t *urg, int scan_times, int skip_scan)
{
    (void)urg;
    (void)scan_times;
    (void)skip_scan;
    // !!!
    return -1;
}


int urg_receive_intensity_measurement(urg_t *urg, long data[], long *timestamp)
{
    (void)urg;
    (void)data;
    (void)timestamp;
    // !!!
    return -1;
}


int urg_start_multiecho_measurement(urg_t *urg, int scan_times, int skip_scan)
{
    (void)urg;
    (void)scan_times;
    (void)skip_scan;
    // !!!
    return -1;
}


int urg_receive_multiecho_measurement(urg_t *urg, long data[], long *timestamp)
{
    (void)urg;
    (void)data;
    (void)timestamp;
    // !!!
    return -1;
}


void urg_stop_measurement(urg_t *urg)
{
    (void)urg;
    // !!!
}


int urg_set_width(urg_t *urg, int first_step, int last_step, int skip_step)
{
    (void)urg;
    (void)first_step;
    (void)last_step;
    (void)skip_step;
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
