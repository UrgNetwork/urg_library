/*!
  \brief URG センサ用の補助関数

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_utils.h"


const char *urg_error(const urg_t *urg)
{
    (void)urg;
    // !!!

    return "Not implemented.";
}


long urg_distance_min(const urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}


long urg_distance_max(const urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}


int urg_width_min(const urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}


int urg_width_max(const urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}


int urg_scan_msec(const urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}


int urg_data_max(const urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}


double urg_index2rad(const urg_t *urg, int index)
{
    (void)urg;
    (void)index;
    // !!!
    return 0.0;
}


int urg_rad2index(const urg_t *urg, double radian)
{
    (void)urg;
    (void)radian;
    // !!!
    return -1;
}


const char *urg_sensor_id(const urg_t *urg)
{
    (void)urg;
    // !!!

    return "Not implemented";
}


const char *urg_sensor_version(const urg_t *urg)
{
    (void)urg;
    // !!!

    return "Not implemented";
}


const char *urg_sensor_status(const urg_t *urg)
{
    (void)urg;
    // !!!

    return "Not implemented";
}
