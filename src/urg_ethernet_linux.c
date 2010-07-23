/*!
  \file
  \brief イーサーネット通信 (Linux)

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_ethernet.h"


int ethernet_open(ethernet_t *ethernet, const char *address, long port)
{
    (void)ethernet;
    (void)address;
    (void)port;
    // !!!

    return -1;
}


void ethernet_close(ethernet_t *ethernet)
{
    (void)ethernet;
    // !!!
}


int ethernet_write(ethernet_t *ethernet, const char *data, int size)
{
    (void)ethernet;
    (void)data;
    (void)size;
    // !!!
    return -1;
}


int ethernet_read(ethernet_t *ethernet, char *data, int max_size, int timeout)
{
    (void)ethernet;
    (void)data;
    (void)max_size;
    (void)timeout;
    // !!!
    return -1;
}


int ethernet_readline(ethernet_t *ethernet, char *data,
                      int max_size, int timeout)
{
    (void)ethernet;
    (void)data;
    (void)max_size;
    (void)timeout;
    // !!!
    return -1;
}


int ethernet_error(ethernet_t *ethernet, char *error_message, int max_size)
{
    (void)ethernet;
    (void)error_message;
    (void)max_size;
    // !!!
    return -1;
}
