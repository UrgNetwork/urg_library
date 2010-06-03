#ifndef URG_ERRNO_H
#define URG_ERRNO_H

/*!
  \file
  \brief URG ライブラリのエラー定義

  \author Satofumi KAMIMURA

  $Id$
*/

enum {
    URG_UNKNOWN_ERROR = -1,
    URG_NOT_CONNECTED = -2,
    URG_NOT_IMPLEMENTED = -3,

    URG_SERIAL_OPEN_ERROR = -4,
    URG_ETHERNET_OPEN_ERROR = -5,

    // !!!
};

#endif /* !URG_ERRNO_H */
