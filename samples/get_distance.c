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
    printf("%ld [mm], (%ld [msec])\n", data[front_index], time_stamp);

#else
    int i;

    // 全てのデータの X-Y の位置を表示
    // !!! 修正する
    printf("# n = %d, time_stamp = %d\n", data_n, time_stamp);
    for (i = 0; i < n; ++i) {
        printf("%d, %ld\n", i, data[i]);
    }
#endif
}


int main(void)
{
    enum {
        CAPTURE_TIMES = 2,
    };
    urg_t urg;
    long *data = NULL;
    long time_stamp;
    //int ret;
    int n;
    int i;

#if defined(URG_WINDOWS_OS)
    const char device[] = "COM4";
#elif defined(URG_LINUX_OS)
    const char device[] = "/dev/ttyACM0";
#else
#endif

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
    ret = urg_set_scanning_parameter(&urg,
                                     urg_deg2step(&urg, -90),
                                     urg_deg2step(&urg, +90), 0);
    if (ret < 0) {
        // !!!
    }
#endif
    //urg_set_scanning_parameter(&urg, 0, 3, 0);

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
