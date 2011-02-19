#include "urg_sensor.h"
#include <stddef.h>

int main(void)
{
    urg_t urg;
    int ret;
    long *length_data = NULL;
// scan_times 回のスキャンデータを取得

// urg_start_measurement() 関数でスキャン回数を指定し
// urg_get_distance() 関数で指定した回数だけデータを受信する。

const int scan_times = 123;
int length_data_size;
int i;

// センサから距離データを取得する。
ret = urg_start_measurement(&urg, URG_DISTANCE, scan_times, 0);
// \todo check error code

for (i = 0; i < scan_times; ++i) {
    length_data_size = urg_get_distance(&urg, length_data, NULL);
    // \todo process length_data array
}
return 0;
}
