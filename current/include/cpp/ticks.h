#ifndef QRK_TICKS_H
#define QRK_TICKS_H

#include <stdint.h>

/*!
  \file
  \~japanese
  \brief タイムスタンプの取得
  \~english
  \brief Gets timestamp values
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

namespace qrk
{
  //! \~japanese 現在のタイムスタンプを取得する  \~english Gets the current timestamp
  int32_t ticks(void);
}

#endif /* !QRK_TICKS_H */
