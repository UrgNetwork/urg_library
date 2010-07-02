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
    switch (communication_type) {
    case URG_SERIAL:
        return serial_open(&communication->serial, device, baudrate_or_port);
        break;

    case URG_ETHERNET:
        return ethernet_open(&communication->ethernet,
                             device, baudrate_or_port);
        break;
    }
    return -1;
}


void communication_close(communication_t *communication)
{
    switch (communication->type) {
    case URG_SERIAL:
        serial_close(&communication->serial);
        break;

    case URG_ETHERNET:
        ethernet_close(&communication->ethernet);
        break;
    }
}


int communication_write(communication_t *communication,
                        const char *data, int size)
{
    switch (communication->type) {
    case URG_SERIAL:
        return serial_write(&communication->serial, data, size);
        break;
    case URG_ETHERNET:
        return ethernet_write(&communication->ethernet, data, size);
        break;
    }
    return -1;
}


int communication_read(communication_t *communication,
                       char *data, int max_size, int timeout)
{
    switch (communication->type) {
    case URG_SERIAL:
        return serial_read(&communication->serial, data, max_size, timeout);
        break;
    case URG_ETHERNET:
        return ethernet_read(&communication->ethernet, data, max_size, timeout);
        break;
    }
    return -1;
}


int communication_readline(communication_t *communication,
                           char *data, int max_size, int timeout)
{
    switch (communication->type) {
    case URG_SERIAL:
        return serial_readline(&communication->serial, data, max_size, timeout);
        break;
    case URG_ETHERNET:
        return ethernet_readline(&communication->ethernet,
                                 data, max_size, timeout);
        break;
    }
    return -1;
}
