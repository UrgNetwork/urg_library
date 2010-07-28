/*!
  \example sync_time_stamp.c センサと PC のタイムスタンプを同期する

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
        TIME_STAMP_RECEIVE_TIMES = 5,
    };

    urg_t urg;
    long time_stamp;
    int i;

#if defined(URG_WINDOWS_OS)
    const char device[] = "COM3";
#elif defined(URG_LINUX_OS)
    const char device[] = "/dev/ttyACM0";
#else
#endif

    // 接続
    if (urg_open(&urg, URG_SERIAL, device, 115200) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }

    // !!!

    // !!!
    urg_start_time_stamp_mode(&urg);
    for (i = 0; i < TIME_STAMP_RECEIVE_TIMES; ++i) {
        time_stamp = urg_time_stamp(&urg);
        if (time_stamp < 0) {
            // !!! error_handling
            // !!!
        }
        // !!!
    }
    urg_stop_time_stamp_mode(&urg);

    // !!! URG のタイムスタンプと PC の時間の関係を出力する

    urg_close(&urg);

    return 0;
}
