/*!
  \example sync_time_stamp.c センサと PC のタイムスタンプを同期する

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
    enum {
        TIME_STAMP_RECEIVE_TIMES = 5,
    };

    urg_t urg;
    connection_type_t connection_type = URG_SERIAL;
    long time_stamp;
    int i;

#if defined(URG_WINDOWS_OS)
    const char device[] = "COM3";
#elif defined(URG_LINUX_OS)
    const char device[] = "/dev/ttyACM0";
#else
#endif

    // 接続タイプの切替え
    for (i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-e")) {
            connection_type = URG_ETHERNET;
        }
    }

    // 接続
    if (urg_open(&urg, connection_type, device, 115200) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }

    // URG のタイムスタンプと PC のタイムスタンプを表示
    // !!!

    // URG のタイムスタンプと PC のタイムスタンプを比較し、
    // 出力結果を同期させるための補正値を計算する
    urg_start_time_stamp_mode(&urg);
    for (i = 0; i < TIME_STAMP_RECEIVE_TIMES; ++i) {
        time_stamp = urg_time_stamp(&urg);
        if (time_stamp < 0) {
            printf("urg_time_stamp: %s\n", urg_error(&urg));
            return 1;
        }
        // !!!
    }
    urg_stop_time_stamp_mode(&urg);

    // URG の補正後のタイムスタンプと PC タイムスタンプを表示
    // !!!

    urg_close(&urg);

    return 0;
}
