/*!
  \example calculate_xy.c X-Y 座標系での位置を計算する

  センサ前方が X 軸の方向とみなした直行座標上で、距離データを位置を出力する。

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
    urg_t urg;
    connection_type_t connection_type = URG_SERIAL;
    long *data;
    long max_distance;
    long min_distance;
    long time_stamp;
    int i;
    int n;

#if defined(URG_WINDOWS_OS)
    const char *device = "COM3";
#elif defined(URG_LINUX_OS)
    const char *device = "/dev/ttyACM0";
#else
#endif
    long baudrate_or_port = 115200;
    const char *ip_address = "192.168.0.10";

    // 接続タイプの切替え
    for (i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-e")) {
            connection_type = URG_ETHERNET;
            baudrate_or_port = 10940;
            if (argc > (i + 1)) {
                device = argv[i + 1];
            } else {
                device = ip_address;
            }
        }
    }

    // 接続
    if (urg_open(&urg, connection_type, device, baudrate_or_port) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }
    data = malloc(urg_max_data_size(&urg) * sizeof(data[0]));

    // データ取得
    urg_start_measurement(&urg, URG_DISTANCE, 1, 0);
    n = urg_get_distance(&urg, data, &time_stamp);
    if (n < 0) {
        printf("urg_distance: %s\n", urg_error(&urg));
        urg_close(&urg);
        return 1;
    }

    // X-Y 座標系の値を出力
    urg_distance_min_max(&urg, &min_distance, &max_distance);
    for (i = 0; i < n; ++i) {
        long distance = data[i];
        double radian;
        long x;
        long y;

        if ((distance < min_distance) || (distance > max_distance)) {
            continue;
        }

        radian = urg_index2rad(&urg, i);
        x = (long)(distance * cos(radian));
        y = (long)(distance * sin(radian));

        printf("%ld, %ld\n", x, y);
    }
    printf("\n");

    // 切断
    free(data);
    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
