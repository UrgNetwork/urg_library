/*!
  \example get_distance.c 距離データを取得して、出力する

  \author Satofumi KAMIMURA

  $Id$

  \todo Windows の場合に COMx が選択されるようにする
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdio.h>


int main(void)
{
    urg_t urg;
    long data[1];
    long timestamp;
    int i;
    int n;

    // !!!

    urg_initialize(&urg);
    if (urg_open(&urg, "/dev/ttyACM0", 115200, URG_SERIAL) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }

    // !!! サイズを取得して malloc するように変更する

    n = urg_get_distance(&urg, data, &timestamp);
    if (n < 0) {
        // !!! error handling
        urg_close(&urg);
        return 1;
    }

    for (i = 0; i < n; ++i) {
        // !!!
        // printf()
    }

    urg_close(&urg);

    // !!! free

    return 0;
}
