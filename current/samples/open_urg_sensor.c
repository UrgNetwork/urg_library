/*!
  \file
  \~japanese
  \brief URG との接続

  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#include "open_urg_sensor.h"
#include "urg_utils.h"
#include "urg_detect_os.h"
#include <string.h>
#include <stdio.h>


int open_urg_sensor(urg_t *urg, int argc, char *argv[])
{
#if defined(URG_WINDOWS_OS)
    const char *device = "COM4";
#elif defined(URG_LINUX_OS)
    //const char *device = "/dev/ttyUSB0";
    const char *device = "/dev/ttyACM0";
#else
#endif
    urg_connection_type_t connection_type = URG_SERIAL;
    long baudrate_or_port = 115200;
    const char *ip_address = "192.168.0.10";
    int i;

    // \~japanese 接続タイプの切替え
    for (i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-e")) {
            connection_type = URG_ETHERNET;
            baudrate_or_port = 10940;
            device = ip_address;
        }
    }

    // \~japanese 接続
    if (urg_open(urg, connection_type, device, baudrate_or_port) < 0) {
        printf("urg_open: %s\n", urg_error(urg));
        return -1;
    }

    return 0;
}
