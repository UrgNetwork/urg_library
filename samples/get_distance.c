/*!
  \example get_distance.c 距離データを取得する

  \author Satofumi KAMIMURA

  $Id$

  \todo Windows の場合に COMx が選択されるようにする
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdio.h>
#include <stdlib.h>


static void print_data(urg_t *urg, long data[], int data_n, long time_stamp)
{
#if 1
    (void)data_n;

    // 前方のデータのみを表示
    int front_index = urg_step2index(urg, 0);
    fprintf(stderr, "front_index: %d\n", front_index);
    printf("%ld, (%ld)\n", data[front_index], time_stamp);

#else
    int i;

    // 全てのデータを表示
    printf("# n = %d, time_stamp = %d\n", data_n, time_stamp);
    for (i = 0; i < n; ++i) {
        printf("%d, %ld\n", i, data[i]);
    }
#endif
}


int main(void)
{
    enum {
        CAPTURE_TIMES = 1,
    };
    urg_t urg;
    long *data = NULL;
    long time_stamp;
    int ret;
    int n;
    int i;

    // 接続
    if (urg_open(&urg, URG_SERIAL, "/dev/ttyACM0", 115200) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }

    data = malloc(urg_max_index(&urg) * sizeof(data[0]));
    if (!data) {
        perror("urg_max_index()");
        return 1;
    }

    // データ取得
    fprintf(stderr, "measure_start: %d\n", urg_deg2index(&urg, -90));
#if 1
    ret = urg_set_scanning_parameter(&urg,
                                     urg_deg2step(&urg, -90),
                                     urg_deg2step(&urg, +90), 0);
    if (ret < 0) {
        // !!!
    }
#endif

    urg_start_measurement(&urg, URG_DISTANCE, CAPTURE_TIMES, 0);
    for (i = 0; i < CAPTURE_TIMES; ++i) {
        n = urg_get_distance(&urg, data, &time_stamp);
        fprintf(stderr, "[n = %d]\n", n);
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
