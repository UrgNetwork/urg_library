#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdio.h>
#include <math.h>

int32_t main(void)
{
    urg_t urg;
    int32_t *length_data = NULL;
    int32_t length_data_size;
    int32_t i;
    // \~japanese 距離データを X-Y 座標系に変換して表示する
    // \~english Converts data to X-Y coordinates and displays it

    length_data_size = urg_get_distance(&urg, length_data, NULL);
    for (i = 0; i < length_data_size; ++i)
    {
        // \~japanese その距離データのラジアン角度を求め、X, Y の座標値を計算する
        // \~english Gets the angle in radians for range data, and convert to X-Y coordinates
        double radian;
        int32_t length;
        int32_t x;
        int32_t y;

        radian = urg_index2rad(&urg, i);
        length = length_data[i];
        // \todo check length is valid

        x = (int32_t)(length * cos(radian));
        y = (int32_t)(length * sin(radian));
        printf("(%ld, %ld), ", x, y);
    }
    printf("\n");
    return 0;
}
