/*!
  \~japanese
  \example get_distance.cpp 距離データを取得する
  \~english
  \example get_distance.cpp Obtains distance data
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#include "Urg_driver.h"
#include "Connection_information.h"
#include "math_utilities.h"
#include <iostream>

using namespace qrk;
using namespace std;

namespace
{
    void print_data(const Urg_driver &urg,
                    const vector<int32_t> &data, int32_t time_stamp)
    {
#if 1
        // \~japanese 前方のデータのみを表示
        // \~english Shows only the front step
        int32_t front_index = urg.step2index(0);
        cout << data[front_index] << " [mm], ("
             << time_stamp << " [msec])" << endl;

#else
        // \~japanese 全てのデータの X-Y の位置を表示
        // \~english Prints the X-Y coordinates for all the measurement points
        int32_t min_distance = urg.min_distance();
        int32_t max_distance = urg.max_distance();
        size_t data_n = data.size();
        for (size_t i = 0; i < data_n; ++i)
        {
            int32_t l = data[i];
            if ((l <= min_distance) || (l >= max_distance))
            {
                continue;
            }

            double radian = urg.index2rad(i);
            int32_t x = static_cast<int32_t>(l * cos(radian));
            int32_t y = static_cast<int32_t>(l * sin(radian));
            cout << "(" << x << ", " << y << ")" << endl;
        }
        cout << endl;
#endif
    }
}

int32_t main(int32_t argc, char *argv[])
{
    Connection_information information(argc, argv);

    // \~japanese 接続
    // \~english Connects to the sensor
    Urg_driver urg;
    if (!urg.open(information.device_or_ip_name(),
                  information.baudrate_or_port_number(),
                  information.connection_type()))
    {
        cout << "Urg_driver::open(): "
             << information.device_or_ip_name() << ": " << urg.what() << endl;
        return 1;
    }

    // \~japanese データ取得
    // \~english Gets measurement data
#if 1
    // \~japanese データの取得範囲を変更する場合
    // \~english Case where the measurement range (start/end steps) is defined
    urg.set_scanning_parameter(urg.deg2step(-90), urg.deg2step(+90), 0);
#endif
    enum
    {
        Capture_times = 10
    };
    urg.start_measurement(Urg_driver::Distance, Urg_driver::Infinity_times, 0);
    for (int32_t i = 0; i < Capture_times; ++i)
    {
        vector<int32_t> data;
        int32_t time_stamp = 0;

        if (!urg.get_distance(data, &time_stamp))
        {
            cout << "Urg_driver::get_distance(): " << urg.what() << endl;
            return 1;
        }
        print_data(urg, data, time_stamp);
    }

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
