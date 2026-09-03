/*!
  \~japanese
  \example get_distance.c 距離データを取得する
  \~english
  \example get_distance.c Obtains distance data
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "open_urg_sensor.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

static void print_data(urg_t *urg, int32_t data[], int32_t data_n, int32_t time_stamp)
{
#if 1
    int32_t front_index;

    (void)data_n;

    // \~japanese 前方のデータのみを表示
    // \~english Shows only the front step
    front_index = urg_step2index(urg, 0);
    printf("%" PRId32 " [mm], (%" PRId32 " [msec])\n", data[front_index], time_stamp);

#else
    (void)time_stamp;

    int32_t i;
    int32_t min_distance;
    int32_t max_distance;

    // \~japanese 全てのデータの X-Y の位置を表示
    // \~english Prints the X-Y coordinates for all the measurement points
    urg_distance_min_max(urg, &min_distance, &max_distance);
    for (i = 0; i < data_n; ++i)
    {
        int32_t l = data[i];
        double radian;
        int32_t x;
        int32_t y;

        if ((l <= min_distance) || (l >= max_distance))
        {
            continue;
        }
        radian = urg_index2rad(urg, i);
        x = (int32_t)(l * cos(radian));
        y = (int32_t)(l * sin(radian));
        printf("(%" PRId32 ", %" PRId32 "), ", x, y);
    }
    printf("\n");
#endif
}

int32_t main(int32_t argc, char *argv[])
{
    enum
    {
        CAPTURE_TIMES = 10,
    };
    urg_t urg;
    int32_t *data = NULL;
    int32_t time_stamp;
    int32_t n;
    int32_t i;

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
#if 0
    // \~japanese データの取得範囲を変更する場合
    // \~english Case where the measurement range (start/end steps) is defined
    urg_set_scanning_parameter(&urg,
                               urg_deg2step(&urg, -90),
                               urg_deg2step(&urg, +90), 0);
#endif

    urg_start_measurement(&urg, URG_DISTANCE, URG_SCAN_INFINITY, 0, 1);
    for (i = 0; i < CAPTURE_TIMES; ++i)
    {
        n = urg_get_distance(&urg, data, &time_stamp);
        if (n <= 0)
        {
            printf("urg_get_distance: %s\n", urg_error(&urg));
            free(data);
            urg_close(&urg);
            return 1;
        }
        print_data(&urg, data, n, time_stamp);
    }

    // \~japanese 切断
    // \~english Disconnects
    free(data);
    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
