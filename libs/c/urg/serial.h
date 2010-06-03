#ifndef SERIAL_H
#define SERIAL_H

/*!
  \file
  \brief シリアル通信

  \author Satofumi KAMIMURA

  $Id$
*/


void serial_open(serial_t *serial, const char *device, long baudrate);
// !!!

#endif /* !SERIAL_H */
