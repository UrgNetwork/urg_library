/*!
  \example sync_timestamp.c URG と PC のタイムスタンプを同期する

  \author Satofumi KAMIMURA

  $Id$

  \todo Windows の場合に COMx が選択されるようにする
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdio.h>


int main(void)
{
    enum {
        TIMESTAMP_RECEIVE_TIMES = 5,
    };

    urg_t urg;
    long timestamp;
    int i;

    // !!!

    urg_initialize(&urg);
    if (urg_open(&urg, "/dev/ttyACM0", 115200, URG_SERIAL) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }

    // !!!

    // !!!
    urg_start_timestamp_mode(&urg);
    for (i = 0; i < TIMESTAMP_RECEIVE_TIMES; ++i) {
        timestamp = urg_timestamp(&urg);
        if (timestamp < 0) {
            // !!! error_handling
            // !!!
        }
        // !!!
    }
    urg_stop_timestamp_mode(&urg);

    // !!! URG のタイムスタンプと PC の時間の関係を出力する

    urg_close(&urg);

    return 0;
}
