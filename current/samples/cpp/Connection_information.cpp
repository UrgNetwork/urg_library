/*!
  \file
  \brief ê⁄ë±èÓïÒÇÃä«óù

  \author Satofumi KAMIMURA

  $Id: Connection_information.cpp 1937 2010-10-25 01:12:49Z satofumi $
*/

#include "Connection_information.h"
#include "detect_os.h"
#include <string>
#include <cstring>

using namespace std;
using namespace qrk;


struct Connection_information::pImpl
{
    Urg_driver::connection_type_t connection_type_;
    string device_or_ip_name_;
    long baudrate_or_port_number_;


    void set_serial_connection(void)
    {
        connection_type_ = Urg_driver::Serial;
#if defined(QRK_WINDOWS_OS)
        device_or_ip_name_ = "COM3";
#elif defined(QRK_LINUX_OS)
        device_or_ip_name_ = "/dev/ttyACM0";
#else
#endif
        baudrate_or_port_number_ = 115200;
    }


    void set_ethernet_connection(void)
    {
        connection_type_ = Urg_driver::Ethernet;
        //device_or_ip_name_ = "localhost";
        device_or_ip_name_ = "192.168.0.10";
        baudrate_or_port_number_ = 10940;
    }
};


Connection_information::Connection_information(int argc,
                                               const char*const argv[])
    : pimpl(new pImpl)
{
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-e")) {
            pimpl->set_ethernet_connection();
            return;
        }
    }
    pimpl->set_serial_connection();
}


Connection_information::~Connection_information(void)
{
}


Urg_driver::connection_type_t
Connection_information::connection_type(void) const
{
    return pimpl->connection_type_;
}


const char* Connection_information::device_or_ip_name(void) const
{
    return pimpl->device_or_ip_name_.c_str();
}


long Connection_information::baudrate_or_port_number(void) const
{
    return pimpl->baudrate_or_port_number_;
}
