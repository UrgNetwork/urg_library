/*!
 \~japanese
 \example get_multiecho.c 距離データ(マルチエコー)を取得する
 \~english
 \example get_multiecho.c Obtains multiecho distance data
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

static void print_echo_data(int32_t data[], int32_t index)
{
    int32_t i;

    // [mm]
    for (i = 0; i < URG_MAX_ECHO; ++i)
    {
        printf("%" PRId32 ", ", data[(URG_MAX_ECHO * index) + i]);
    }
}

static void print_data(urg_t *urg, int32_t data[], int32_t data_n, int32_t time_stamp)
{
#if 1
    int32_t front_index;

    (void)data_n;

    // \~japanese 前方のデータのみを表示
    // \~english Shows only the front step
    front_index = urg_step2index(urg, 0);
    print_echo_data(data, front_index);
    printf("%" PRId32 "\n", time_stamp);

#else
    (void)urg;

    int32_t i;

    // \~japanese 全てのデータを表示
    // \~english Prints the multiecho distance for all the measurement points
    printf("# n = %d, time_stamp = %" PRId32 "\n", data_n, time_stamp);
    for (i = 0; i < data_n; ++i)
    {
        print_echo_data(data, i);
        printf("\n");
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
    int32_t *data = NULL;
    int32_t time_stamp;
    int32_t n;
    int32_t i;

    if (open_urg_sensor(&urg, argc, argv) < 0)
    {
        return 1;
    }

    data = (int32_t *)malloc(urg_max_data_size(&urg) * 3 * sizeof(data[0]));
    if (!data)
    {
        perror("urg_max_index()");
        return 1;
    }

    // \~japanese データ取得
    // \~english Gets measurement data
    urg_start_measurement(&urg, URG_MULTIECHO, URG_SCAN_INFINITY, 0, 1);
    for (i = 0; i < CAPTURE_TIMES; ++i)
    {
        n = urg_get_multiecho(&urg, data, &time_stamp);
        if (n <= 0)
        {
            printf("urg_get_multiecho: %s\n", urg_error(&urg));
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
