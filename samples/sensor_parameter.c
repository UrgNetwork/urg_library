/*!
  \example sensor_parameter.c センサ情報の出力

  \author Satofumi KAMIMURA

  $Id$

  \todo Windows の場合に COMx が選択されるようにする
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
    urg_t urg;
    connection_type_t connection_type = URG_SERIAL;
    int min_step;
    int max_step;
    long min_distance;
    long max_distance;
    int i;

#if defined(URG_WINDOWS_OS)
    const char device[] = "COM3";
#elif defined(URG_LINUX_OS)
    const char device[] = "/dev/ttyACM0";
#else
#endif

    // 接続タイプの切替え
    for (i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-e")) {
            connection_type = URG_ETHERNET;
        }
    }

    if (urg_open(&urg, URG_SERIAL, device, 115200) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }

    printf("Sensor firmware version: %s\n", urg_sensor_version(&urg));
    printf("Sensor serial ID: %s\n", urg_sensor_id(&urg));
    printf("Sensor status: %s\n", urg_sensor_status(&urg));

    urg_step_min_max(&urg, &min_step, &max_step);
    printf("step: [%d, %d]\n", min_step, max_step);

    urg_distance_min_max(&urg, &min_distance, &max_distance);
    printf("distance: [%ld, %ld)\n", min_distance, max_distance);

    printf("scan interval: %ld [usec]\n", urg_scan_usec(&urg));
    printf("sensor data size: %d\n", urg_max_data_size(&urg));

    urg_close(&urg);

    return 0;
}
