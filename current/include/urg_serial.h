#ifndef URG_SERIAL_H
#define URG_SERIAL_H

/*!
  \file
  \brief シリアル通信

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
    char buffer[RING_BUFFER_SIZE];
    char has_last_ch;          /*!< 書き戻した文字があるかのフラグ */
    char last_ch;              /*!< 書き戻した１文字 */
} urg_serial_t;


extern int serial_open(urg_serial_t *serial, const char *device, long baudrate);
extern void serial_close(urg_serial_t *serial);
extern int serial_set_baudrate(urg_serial_t *serial, long baudrate);
extern int serial_write(urg_serial_t *serial, const char *data, int size);
extern int serial_read(urg_serial_t *serial,
                       char *data, int max_size, int timeout);
extern int serial_readline(urg_serial_t *serial,
                           char *data, int max_size, int timeout);
extern int serial_error(urg_serial_t *serial, char *error_message, int max_size);

#endif /* !URG_SERIAL_H */
