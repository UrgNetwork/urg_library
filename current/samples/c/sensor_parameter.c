/*!
  \~japanese
  \example sensor_parameter.c センサ情報の出力
  \~english
  \example sensor_parameter.c Outputs sensor information fields
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "open_urg_sensor.h"
#include <stdio.h>
#include <inttypes.h>

int32_t main(int32_t argc, char *argv[])
{
    urg_t urg;
    int32_t min_step;
    int32_t max_step;
    int32_t min_distance;
    int32_t max_distance;

    if (open_urg_sensor(&urg, argc, argv) < 0)
    {
        return 1;
    }

    printf("Sensor product type: %s\n", urg_sensor_product_type(&urg));
    printf("Sensor firmware version: %s\n", urg_sensor_firmware_version(&urg));
    printf("Sensor serial ID: %s\n", urg_sensor_serial_id(&urg));
    printf("Sensor status: %s\n", urg_sensor_status(&urg));
    printf("Sensor state: %s\n", urg_sensor_state(&urg));

    urg_step_min_max(&urg, &min_step, &max_step);
    printf("step: [%d, %d]\n", min_step, max_step);

    urg_distance_min_max(&urg, &min_distance, &max_distance);
    printf("distance: [%" PRId32 ", %" PRId32 ")\n", min_distance, max_distance);

    printf("scan interval: %" PRId32 " [usec]\n", urg_scan_usec(&urg));
    printf("sensor data size: %d\n", urg_max_data_size(&urg));

    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
