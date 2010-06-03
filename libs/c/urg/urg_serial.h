#ifndef URG_SERIAL_H
#define URG_SERIAL_H

/*!
  \file
  \brief シリアル通信

  \author Satofumi KAMIMURA

  $Id$
*/


void serial_open(serial_t *serial, const char *device, long baudrate);
// !!!

#endif /* !URG_SERIAL_H */
