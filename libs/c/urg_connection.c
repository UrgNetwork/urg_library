/*!
  \file
  \brief 通信の処理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_connection.h"


int connection_open(connection_t *connection,
                    connection_type_t connection_type,
                    const char *device, long baudrate_or_port)
{
    switch (connection_type) {
    case URG_SERIAL:
        return serial_open(&connection->serial, device, baudrate_or_port);
        break;

    case URG_ETHERNET:
        return ethernet_open(&connection->ethernet,
                             device, baudrate_or_port);
        break;
    }
    return -1;
}


void connection_close(connection_t *connection)
{
    switch (connection->type) {
    case URG_SERIAL:
        serial_close(&connection->serial);
        break;

    case URG_ETHERNET:
        ethernet_close(&connection->ethernet);
        break;
    }
}


int connection_set_baudrate(connection_t *connection, long baudrate)
{
    switch (connection->type) {
    case URG_SERIAL:
        return serial_set_baudrate(&connection->serial, baudrate);
        break;
    case URG_ETHERNET:
        return 0;
        break;
    }
    return -1;
}


int connection_write(connection_t *connection,
                     const char *data, int size)
{
    switch (connection->type) {
    case URG_SERIAL:
        return serial_write(&connection->serial, data, size);
        break;
    case URG_ETHERNET:
        return ethernet_write(&connection->ethernet, data, size);
        break;
    }
    return -1;
}


int connection_read(connection_t *connection,
                    char *data, int max_size, int timeout)
{
    switch (connection->type) {
    case URG_SERIAL:
        return serial_read(&connection->serial, data, max_size, timeout);
        break;
    case URG_ETHERNET:
        return ethernet_read(&connection->ethernet, data, max_size, timeout);
        break;
    }
    return -1;
}


int connection_readline(connection_t *connection,
                        char *data, int max_size, int timeout)
{
    switch (connection->type) {
    case URG_SERIAL:
        return serial_readline(&connection->serial, data, max_size, timeout);
        break;
    case URG_ETHERNET:
        return ethernet_readline(&connection->ethernet,
                                 data, max_size, timeout);
        break;
    }
    return -1;
}
