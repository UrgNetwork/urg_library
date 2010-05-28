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


void urg_distance_min_max(const urg_t *urg,
                          long *min_distance, long *max_distance)
{
    (void)urg;
    (void)min_distance;
    (void)max_distance;

    // !!! urg_set_communication_data_size() の設定も反映する

    // !!!
}


void urg_step_min_max(const urg_t *urg, int *min_index, int *max_index)
{
    (void)urg;
    (void)min_index;
    (void)max_index;

    // !!!
}


int urg_scan_msec(const urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}


int urg_max_index(const urg_t *urg)
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


double urg_index2deg(const urg_t *urg, int index)
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

    return 0;
}


int urg_deg2index(const urg_t *urg, double degree)
{
    (void)urg;
    (void)degree;
    // !!!

    return 0;
}


int urg_rad2step(const urg_t *urg, double radian)
{
    (void)urg;
    (void)radian;

    // !!!

    return 0;
}


int urg_deg2step(const urg_t *urg, double degree)
{
    (void)urg;
    (void)degree;

    // !!!

    return 0;
}


double urg_step2rad(const urg_t *urg, int step)
{
    (void)urg;
    (void)step;

    // !!!

    return 0.0;
}


int urg_step2deg(const urg_t *urg, int step)
{
    (void)urg;
    (void)step;

    // !!!

    return 0;
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


int urg_find_port(char *port_name, int index)
{
    (void)port_name;
    (void)index;

    // !!!

    return 0;
}
