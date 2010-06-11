/*!
  \file
  \brief 通信の処理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_communication.h"


int communication_open(communication_t *communication,
                       communication_type_t communication_type,
                       const char *device, long baudrate_or_port)
{
    (void)communication;
    (void)communication_type;
    (void)device;
    (void)baudrate_or_port;

    // !!!

    return -1;
}


void communication_close(communication_t *communication)
{
    (void)communication;
    // !!!
}


int communication_write(communication_t *communication,
                        const char *data, int size)
{
    (void)communication;
    (void)data;
    (void)size;

    // !!!

    return -1;
}


int communication_read(communication_t *communication,
                       char *data, int max_size, int timeout)
{
    (void)communication;
    (void)data;
    (void)max_size;
    (void)timeout;

    // !!!

    return -1;
}


int communication_readline(communication_t *communication,
                           char *data, int max_size, int timeout)
{
    (void)communication;
    (void)data;
    (void)max_size;
    (void)timeout;
    // !!!

    return -1;
}
