#ifndef URG_SERIAL_T_H
#define URG_SERIAL_T_H

/*!
  \file
  \brief シリアル通信管理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_detect_os.h"
#if defined(URG_WINDOWS_OS)
#include <windows.h>
#else
#include <termios.h>
#endif
#include "urg_ring_buffer.h"

enum {
    RING_BUFFER_SIZE_SHIFT = 7,
    RING_BUFFER_SIZE = 1 << RING_BUFFER_SIZE_SHIFT,

    ERROR_MESSAGE_SIZE = 256,
};


typedef struct
{
#if defined(URG_WINDOWS_OS)
    HANDLE hCom;                /*!< 接続リソース */
    int current_timeout;        /*!< タイムアウトの設定時間 [msec] */
#else
    int fd;
    struct termios sio;
#endif

    ring_buffer_t ring;         /*!< リングバッファ */
    char buffer[RING_BUFFER_SIZE_SHIFT];
    char has_last_ch;          /*!< 書き戻した文字があるかのフラグ */
    char last_ch;              /*!< 書き戻した１文字 */
    char error_string[ERROR_MESSAGE_SIZE];
} serial_t;

#endif /* !URG_SERIAL_T_H */
