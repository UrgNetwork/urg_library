#include "urg_sensor.h"
#include <stdio.h>

int32_t main(void)
{
    urg_t urg;
    int32_t *length_data = NULL;
    int32_t ret;
    // \~japanese タイムスタンプの取得
    // \~english Gets timestamp values

    // \~japanese urg_get_distance() 関数に変数を与え、タイムスタンプを取得する。
    // \~english Uses the urg_get_distance() function and returns the timestamp values for each scan

    const int32_t scan_times = 123;
    int32_t length_data_size;
    int32_t timestamp;
    int32_t i;

    // \~japanese センサから距離データを取得する。
    // \~english Starts range data measurement
    ret = urg_start_measurement(&urg, URG_DISTANCE, scan_times, 0, 0);
    // \todo check error code

    for (i = 0; i < scan_times; ++i)
    {
        length_data_size = urg_get_distance(&urg, length_data, &timestamp);
        // \todo process length_data array

        // \~japanese 取得したタイムスタンプを出力する
        // \~english Outputs the received timestamp value
        printf("%ld\n", timestamp);
    }
    return 0;
}
