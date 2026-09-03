/*!
  \~japanese
  \example calculate_xy.c X-Y 座標系での位置を計算する

  センサ前方が X 軸の方向とみなした直行座標上で、距離データを位置を出力する。

  \~english
  \example calculate_xy.c Calculates X-Y coordinates

  Having the X axis aligned to the front step of the sensor, calculates the coordinates for measurement data
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "open_urg_sensor.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int32_t main(int32_t argc, char *argv[])
{
    urg_t urg;
    int32_t *data;
    int32_t max_distance;
    int32_t min_distance;
    int32_t time_stamp;
    int32_t i;
    int32_t n;

    if (open_urg_sensor(&urg, argc, argv) < 0)
    {
        return 1;
    }

    data = (int32_t *)malloc(urg_max_data_size(&urg) * sizeof(data[0]));
    if (!data)
    {
        perror("urg_max_index()");
        return 1;
    }

    // \~japanese データ取得
    // \~english Gets measurement data
    urg_start_measurement(&urg, URG_DISTANCE, 1, 0, 1);
    n = urg_get_distance(&urg, data, &time_stamp);
    if (n < 0)
    {
        printf("urg_get_distance: %s\n", urg_error(&urg));
        urg_close(&urg);
        return 1;
    }

    // \~japanese X-Y 座標系の値を出力
    // \~english Outputs X-Y coordinates
    urg_distance_min_max(&urg, &min_distance, &max_distance);
    for (i = 0; i < n; ++i)
    {
        int32_t distance = data[i];
        double radian;
        int32_t x;
        int32_t y;

        if ((distance < min_distance) || (distance > max_distance))
        {
            continue;
        }

        radian = urg_index2rad(&urg, i);
        x = (int32_t)(distance * cos(radian));
        y = (int32_t)(distance * sin(radian));

        printf("%" PRId32 ", %" PRId32 "\n", x, y);
    }
    printf("\n");

    // \~japanese 切断
    // \~english Disconnects
    free(data);
    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
