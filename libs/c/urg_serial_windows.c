/*!
  \file
  \brief シリアル通信

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_serial.h"


int serial_open(serial_t *serial, const char *device, long baudrate)
{
    (void)serial;
    (void)device;
    (void)baudrate;
    // !!!

    return -1;
}


void serial_close(serial_t *serial)
{
    (void)serial;
    // !!!
}


int serial_write(serial_t *serial, const char *data, int size)
{
    (void)serial;
    (void)data;
    (void)size;
    // !!!

    return -1;
}


int serial_read(serial_t *serial, char *data, int max_size, int timeout)
{
    (void)serial;
    (void)data;
    (void)max_size;
    (void)timeout;
    // !!!

    return -1;
}


int serial_readline(serial_t *serial, char *data, int max_size, int timeout)
{
    (void)serial;
    (void)data;
    (void)max_size;
    (void)timeout;
    // !!!

    return -1;
}
