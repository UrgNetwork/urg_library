#ifndef URG_SERIAL_H
#define URG_SERIAL_H

/*!
  \file
  \brief シリアル通信

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_serial_t.h"


int serial_open(serial_t *serial, const char *device, long baudrate);
void serial_close(serial_t *serial);
int serial_write(serial_t *serial, const char *data, int size);
int serial_read(serial_t *serial, char *data, int max_size, int timeout);
int serial_readline(serial_t *serial, char *data, int max_size, int timeout);

#endif /* !URG_SERIAL_H */
