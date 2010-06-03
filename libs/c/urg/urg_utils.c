/*!
  \brief URG センサ用の補助関数

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_utils.h"
#include "urg_errno.h"

#undef max

static const char NOT_CONNECTED_MESSAGE[] = "not connected.";


static int max(int a, int b)
{
    return (a > b) ? a : b;
}


const char *urg_error(const urg_t *urg)
{
    typedef struct
    {
        int no;
        const char* message;
    } error_messages_t;


    error_messages_t errors[] = {
        { URG_NO_ERROR, "Not implemented." },
        { URG_UNKNOWN_ERROR, "Not implemented." },
        { URG_NOT_CONNECTED, "Not implemented." },
        { URG_NOT_IMPLEMENTED, "Not implemented." },
        { URG_INVALID_RESPONSE, "Not implemented." },
        { URG_RECEIVE_ERROR, "Not implemented." },
        { URG_CHECKSUM_ERROR, "Not implemented." },

        { URG_SERIAL_OPEN_ERROR, "Not implemented." },
        { URG_ETHERNET_OPEN_ERROR, "Not implemented." },
        { URG_SCANNING_PARAMETER_ERROR, "Not implemented." },
        { URG_DATA_SIZE_PARAMETER_ERROR, "Not implemented." },
    };

    int n = sizeof(errors) / sizeof(errors[0]);
    int i;

    for (i = 0; i < n; ++i) {
        if (errors[i].no == urg->last_errno) {
            return errors[i].message;
        }
    }

    return "Unknown error.";
}


void urg_distance_min_max(const urg_t *urg,
                          long *min_distance, long *max_distance)
{
    if (!urg->is_active) {
        *min_distance = 1;
        *max_distance = 0;
        return;
    }

    *min_distance = urg->min_distance;

    // urg_set_communication_data_size() を反映した距離を返す
    *max_distance =
        (urg->communication_data_size == URG_COMMUNICATION_2_BYTE) ?
        max(urg->max_distance, 4095) : urg->max_distance;
}


void urg_step_min_max(const urg_t *urg, int *min_index, int *max_index)
{
    if (!urg->is_active) {
        *min_index = 1;
        *max_index = 0;
        return;
    }

    (void)urg;
    (void)min_index;
    (void)max_index;

    // !!!
}


long urg_scan_usec(const urg_t *urg)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    return urg->scan_usec;
}


int urg_max_index(const urg_t *urg)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    // !!!

    return -1;
}


double urg_index2rad(const urg_t *urg, int index)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)index;
    // !!!
    return 0.0;
}


double urg_index2deg(const urg_t *urg, int index)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)index;

    // !!!

    return 0.0;
}


int urg_rad2index(const urg_t *urg, double radian)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)radian;

    // !!!

    return 0;
}


int urg_deg2index(const urg_t *urg, double degree)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)degree;
    // !!!

    return 0;
}


int urg_rad2step(const urg_t *urg, double radian)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)radian;

    // !!!

    return 0;
}


int urg_deg2step(const urg_t *urg, double degree)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)degree;

    // !!!

    return 0;
}


double urg_step2rad(const urg_t *urg, int step)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)step;

    // !!!

    return 0.0;
}


int urg_step2deg(const urg_t *urg, int step)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)step;

    // !!!

    return 0;
}


int urg_step2index(const urg_t *urg, int step)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    (void)urg;
    (void)step;
    // !!!

    return 0;
}


const char *urg_sensor_id(const urg_t *urg)
{
    if (!urg->is_active) {
        return NOT_CONNECTED_MESSAGE;
    }

    (void)urg;
    // !!!

    // !!! urg 中に文字列で保持しておき、それを返す

    return "Not implemented";
}


const char *urg_sensor_version(const urg_t *urg)
{
    if (!urg->is_active) {
        return NOT_CONNECTED_MESSAGE;
    }

    (void)urg;
    // !!!

    // !!! urg 中に文字列で保持しておき、それを返す

    return "Not implemented";
}


const char *urg_sensor_status(const urg_t *urg)
{
    if (!urg->is_active) {
        return NOT_CONNECTED_MESSAGE;
    }

    (void)urg;
    // !!!

    // !!! urg 中に文字列で保持しておき、それを返す

    return "Not implemented";
}


int urg_find_port(char *port_name, int index)
{
    (void)port_name;
    (void)index;

    // !!!

    return 0;
}
