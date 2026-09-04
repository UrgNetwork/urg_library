/*!
  \~japanese
  \example get_distance_intensity.c 距離・強度データを取得する
  \~english
  \example get_distance_intensity.c Obtains distance and intensity data
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "open_urg_sensor.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

static void print_data(urg_t *urg, int32_t data[], unsigned short intensity[],
                       int32_t data_n, int32_t time_stamp)
{
#if 1
    int32_t front_index;
    (void)data_n;

    // \~japanese 前方のデータのみを表示
    // \~english Shows only the front step
    front_index = urg_step2index(urg, 0);
    printf("%" PRId32 " [mm], %d [1], (%" PRId32 " [msec])\n",
           data[front_index], intensity[front_index], time_stamp);

#else
    (void)urg;

    int32_t i;

    // \~japanese 全てのデータを表示
    // \~english Prints the range/intensity values for all the measurement points
    printf("# n = %d, time_stamp = %" PRId32 "\n", data_n, time_stamp);
    for (i = 0; i < data_n; ++i)
    {
        printf("%d, %" PRId32 ", %d\n", i, data[i], intensity[i]);
    }
#endif
}

int32_t main(int32_t argc, char *argv[])
{
    enum
    {
        CAPTURE_TIMES = 10,
    };
    urg_t urg;
    int32_t max_data_size;
    int32_t *data = NULL;
    unsigned short *intensity = NULL;
    int32_t time_stamp;
    int32_t n;
    int32_t i;

    if (open_urg_sensor(&urg, argc, argv) < 0)
    {
        return 1;
    }

    max_data_size = urg_max_data_size(&urg);
    data = (int32_t *)malloc(max_data_size * sizeof(data[0]));
    if (!data)
    {
        perror("urg_max_index()");
        return 1;
    }
    intensity = malloc(max_data_size * sizeof(intensity[0]));
    if (!intensity)
    {
        perror("urg_max_index()");
        return 1;
    }

    // \~japanese データ取得
    // \~english Gets measurement data
    urg_start_measurement(&urg, URG_DISTANCE_INTENSITY, URG_SCAN_INFINITY, 0, 1);
    for (i = 0; i < CAPTURE_TIMES; ++i)
    {
        n = urg_get_distance_intensity(&urg, data, intensity, &time_stamp);
        if (n <= 0)
        {
            printf("urg_get_distance_intensity: %s\n", urg_error(&urg));
            free(data);
            urg_close(&urg);
            return 1;
        }
        print_data(&urg, data, intensity, n, time_stamp);
    }

    // \~japanese 切断
    // \~english Disconnects
    free(intensity);
    free(data);
    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
