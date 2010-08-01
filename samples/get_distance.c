/*!
  \example get_distance.c 距離データを取得する

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void print_data(urg_t *urg, long data[], int data_n, long time_stamp)
{
#if 1
    (void)data_n;

    // 前方のデータのみを表示
    int front_index = urg_step2index(urg, 0);
    printf("%ld [mm], (%ld [msec])\n", data[front_index], time_stamp);

#else
    int i;
    int min_distance;
    int max_distance;

    // 全てのデータの X-Y の位置を表示
    urg_distance_min_max(urg, &min_distance, &max_distance);
    for (i = 0; i < data_n; ++i) {
        long l = data[i];
        double radian;
        long x;
        long y;

        if ((l <= min_distance) || (l >= max_distance)) {
            continue;
        }
        radian = urg_index2rad(urg);
        x = (long)(l * cos(radian));
        y = (long)(l * sin(radian));
        printf("(%d, %d), ", x, y);
    }
    printf("\n");
#endif
}


int main(int argc, char *argv[])
{
    enum {
        CAPTURE_TIMES = 10,
    };
    urg_t urg;
    connection_type_t connection_type = URG_SERIAL;
    long *data = NULL;
    long time_stamp;
    int n;
    int i;

#if defined(URG_WINDOWS_OS)
    const char device[] = "COM4";
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

    // 接続
    if (urg_open(&urg, URG_SERIAL, device, 115200) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }

    data = malloc(urg_max_data_size(&urg) * sizeof(data[0]));
    if (!data) {
        perror("urg_max_index()");
        return 1;
    }

    // データ取得
#if 0
    // データの取得範囲を変更する場合
    urg_set_scanning_parameter(&urg,
                               urg_deg2step(&urg, -90),
                               urg_deg2step(&urg, +90), 0);
#endif

    urg_start_measurement(&urg, URG_DISTANCE, CAPTURE_TIMES, 0);
    for (i = 0; i < CAPTURE_TIMES; ++i) {
        n = urg_get_distance(&urg, data, &time_stamp);
        if (n <= 0) {
            printf("urg_distance: %s\n", urg_error(&urg));
            free(data);
            urg_close(&urg);
            return 1;
        }
        print_data(&urg, data, n, time_stamp);
    }

    // 切断
    free(data);
    urg_close(&urg);

    return 0;
}
