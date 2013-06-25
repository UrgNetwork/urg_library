/*!
  \example sensor_parameter.c センサ情報の出力

  \author Satofumi KAMIMURA

  $Id: sensor_parameter.cpp 1937 2010-10-25 01:12:49Z satofumi $
*/

#include "Urg_driver.h"
#include "Connection_information.h"
#include <iostream>

using namespace qrk;
using namespace std;


int main(int argc, char *argv[])
{
    Connection_information information(argc, argv);

    // 接続
    Urg_driver urg;
    if (!urg.open(information.device_or_ip_name(),
                  information.baudrate_or_port_number(),
                  information.connection_type())) {
        cout << "Urg_driver::open():"
             << information.device_or_ip_name() << ": " << urg.what() << endl;
        return 1;
    }

    // パラメータ情報の表示
    cout << "Sensor product type: " << urg.product_type() << endl;
    cout << "Sensor firmware version: " << urg.firmware_version() << endl;
    cout << "Sensor serial ID: " << urg.serial_id() << endl;
    cout << "Sensor status: " << urg.status() << endl;
    cout << "Sensor state: " << urg.state() << endl;

    cout << "step: ["
         << urg.min_step() << ", "
         << urg.max_step() << "]" << endl;
    cout << "distance: ["
         << urg.min_distance()
         << ", " << urg.max_distance() << endl;

    cout << "scan interval: " << urg.scan_usec() << " [usec]" << endl;
    cout << "sensor data size: " << urg.max_data_size() << endl;

    urg.close();

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
