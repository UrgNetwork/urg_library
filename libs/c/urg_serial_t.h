#ifndef URG_SERIAL_T_H
#define URG_SERIAL_T_H

/*!
  \file
  \brief シリアル通信管理

  \author Satofumi KAMIMURA

  $Id$
*/


#if 0
#else
#include "urg_ring_buffer.h"
#include <termios.h>
#endif

enum {
    RING_BUFFER_SIZE_SHIFT = 7,
    RING_BUFFER_SIZE = 1 << RING_BUFFER_SIZE_SHIFT,

    ERROR_MESSAGE_SIZE = 256,
};


typedef struct
{
#if 0
    // !!!
#else
    int fd;
    struct termios sio;
    ring_buffer_t ring;
    char buffer[RING_BUFFER_SIZE_SHIFT];
    char error_string[ERROR_MESSAGE_SIZE];
    char has_last_ch;          /*!< 書き戻した文字があるかのフラグ */
    char last_ch;              /*!< 書き戻した１文字 */
#endif
    // !!!
} serial_t;

#endif /* !URG_SERIAL_T_H */
