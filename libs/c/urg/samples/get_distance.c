/*!
  \example get_distance.c データを逐次取得する

  \author Satofumi KAMIMURA

  $Id$

  \todo Windows の場合に COMx が選択されるようにする
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdio.h>
#include <stdlib.h>


static void printData(urg_t *urg, long data[], int n, long timestamp)
{
    if (n <= 0) {
        return;
    }

#if 1
    // 前方のデータのみを表示
    int front_step = urg_deg2index(urg, 0);
    printf("%ld, (%ld)\n", data[front_step], timestamp);

#else
    int i;

    // 全てのデータを表示
    printf("# n = %d, timestamp = %d\n", n, timestamp);
    for (i = 0; i < n; ++i) {
        printf("%d, %ld\n", i, data[i]);
    }
    printf("\n");
#endif
}


int main(void)
{
    enum {
        CAPTURE_TIMES = 3,
    };
    urg_t urg;
    long *data;
    long timestamp;
    int n;
    int i;

    // 接続
    if (urg_open(&urg, "/dev/ttyACM0", 115200, URG_SERIAL) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }
    data = malloc(urg_max_index(&urg) * sizeof(data[0]));
    if (! data) {
        perror("urg_max_index()");
        return 1;
    }

    // データ取得
    urg_start_measurement(&urg, URG_DISTANCE, CAPTURE_TIMES, 0);
    for (i = 0; i < CAPTURE_TIMES; ++i) {
        n = urg_get_distance(&urg, data, &timestamp);
        if (n < 0) {
            printf("urg_distance: %s\n", urg_error(&urg));
            urg_close(&urg);
            return 1;
        }
        printData(&urg, data, n, timestamp);
    }

    // 切断
    urg_close(&urg);

    return 0;
}
