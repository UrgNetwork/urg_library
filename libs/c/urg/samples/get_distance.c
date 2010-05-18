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


int main(void)
{
    enum {
        CAPTURE_TIMES = 3,
    };
    urg_t urg;
    long *data;
    long timestamp;
    int i;
    int n;

    // 接続
    urg_initialize(&urg);
    if (urg_open(&urg, "/dev/ttyACM0", 115200, URG_SERIAL) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }
    data = malloc(urg_data_max(&urg) * sizeof(data[0]));

    // データ取得
    urg_laser_on(&urg);
    for (i = 0; i < CAPTURE_TIMES; ++i) {
        n = urg_get_distance(&urg, data, &timestamp);
        if (n < 0) {
            printf("urg_distance: %s\n", urg_error(&urg));
            urg_close(&urg);
            return 1;
        }

        for (i = 0; i < n; ++i) {
            printf("%d, %ld\n", i, data[i]);
        }
        printf("\n");
    }

    // 切断
    urg_close(&urg);

    return 0;
}
