#ifndef URG_UTILS_H
#define URG_UTILS_H

/*!
  \file
  \brief URG センサ用の補助関数

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"


/*!
  \brief URG のエラーを示す文字列を返す

  \param[in] urg URG センサ管理

  \retval URG のエラーを示す文字列

  Example
  \code
  if (!urg_open("/dev/ttyACM0", 115200)) {
      printf("urg_connect: %s\n", urg_error(&urg));
      return -1;
  } \endcode
*/
extern const char *urg_error(urg_t *urg);


#endif /* !URG_UTILS_H */
