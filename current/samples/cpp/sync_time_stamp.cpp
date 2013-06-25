/*!
  \example sync_time_stamp.cpp センサから得られるタイムスタンプを PC と同期する

  \author Satofumi KAMIMURA

  $Id: sync_time_stamp.cpp 1937 2010-10-25 01:12:49Z satofumi $
*/

#include "Urg_driver.h"
#include "Connection_information.h"
#include "ticks.h"
#include <iostream>

using namespace qrk;
using namespace std;


namespace
{
    void print_timestamp(Urg_driver& urg)
    {
        enum { Print_times = 3 };

        urg.start_measurement(Urg_driver::Distance, Print_times);
        for (int i = 0; i < Print_times; ++i) {
            long time_stamp;
            vector<long> data;
            urg.get_distance(data, &time_stamp);

            cout << ticks() << ", " << time_stamp << endl;
        }
    }
}


int main(int argc, char *argv[])
{
    Connection_information information(argc, argv);

    // 接続
    Urg_driver urg;
    if (!urg.open(information.device_or_ip_name(),
                  information.baudrate_or_port_number(),
                  information.connection_type())) {
        cout << "Urg_driver::open(): "
             << information.device_or_ip_name() << ": " << urg.what() << endl;
        return 1;
    }

    // データは１ステップのみ取得する
    int min_step = urg.min_step();
    urg.set_scanning_parameter(min_step, min_step);

    // 比較用に PC とセンサのタイムスタンプを表示する
    print_timestamp(urg);
    cout << endl;

    // センサに PC のタイムスタンプを設定し、
    // 距離データを取得したときに得られるタイムスタンプが、
    // PC から得られるタイムスタンプと同じになるようにする
    urg.set_sensor_time_stamp(ticks());

    // 設定後に PC とセンサのタイムスタンプを表示する
    print_timestamp(urg);

    return 0;
}
