/*!
  \file
  \brief 通信の処理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_connection.h"


int connection_open(connection_t *connection, connection_type_t connection_type,
                    const char *device, long baudrate_or_port)
{
    (void)connection;
    (void)connection_type;
    (void)device;
    (void)baudrate_or_port;

    // !!!

    return -1;
}


void connection_close(connection_t *connection)
{
    (void)connection;
    // !!!
}


int connection_write(connection_t *connection, const char *data, int size)
{
    (void)connection;
    (void)data;
    (void)size;

    // !!!

    return -1;
}


int connection_read(connection_t *connection,
                    char *data, int max_size, int timeout)
{
    (void)connection;
    (void)data;
    (void)max_size;
    (void)timeout;

    // !!!

    return -1;
}


int connection_readline(connection_t *connection,
                        char *data, int max_size, int timeout)
{
    (void)connection;
    (void)data;
    (void)max_size;
    (void)timeout;
    // !!!

    return -1;
}
