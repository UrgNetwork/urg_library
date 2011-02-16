// シリアル接続でのセンサとの接続と距離データの取得

#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdlib.h>


int main(void)
{
    urg_t urg;
    int ret;
    long *length_data;
    int length_data_size;

    // "COM1" は、センサが認識されているデバイス名にする必要がある
    const char connect_device[] = "COM1";
    const long connect_baudrate = 115200;

    // センサに対して接続を行う。
    ret = urg_open(&urg, URG_SERIAL, connect_device, connect_baudrate);
    // \todo check error code

    // データ受信のための領域を確保する
    length_data = (long *)malloc(sizeof(long) * urg_max_data_size(&urg));
    // \todo check length_data is not NULL

    // センサから距離データを取得する。
    ret = urg_start_measurement(&urg, URG_DISTANCE, 1, 0);
    // \todo check error code

    length_data_size = urg_get_distance(&urg, length_data, NULL);
    // \todo process length_data array

    // センサとの接続を閉じる。
    urg_close(&urg);

    return 0;
}
