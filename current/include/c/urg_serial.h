#ifndef URG_SERIAL_H
#define URG_SERIAL_H

/*!
  \file
  \brief シリアル通信

  \author Satofumi KAMIMURA

  $Id: urg_serial.h,v 1d233c7a2240 2011/02/19 03:08:45 Satofumi $
*/

#ifdef __cplusplus
extern "C" {
#endif

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


//! シリアル通信用
typedef struct
{
#if defined(URG_WINDOWS_OS)
    HANDLE hCom;                /*!< 接続リソース */
    int current_timeout;        /*!< タイムアウトの設定時間 [msec] */
#else
    int fd;                     /*!< ファイルディスクリプタ*/
    struct termios sio;         /*!< 通信設定 */
#endif

    ring_buffer_t ring;         /*!< リングバッファ */
    char buffer[RING_BUFFER_SIZE]; /*!< バッファ領域 */
    char has_last_ch;          /*!< 書き戻した文字があるかのフラグ */
    char last_ch;              /*!< 書き戻した１文字 */
} urg_serial_t;


//! 接続を開く
extern int serial_open(urg_serial_t *serial, const char *device, long baudrate);


//! 接続を閉じる
extern void serial_close(urg_serial_t *serial);


//! ボーレートを設定する
extern int serial_set_baudrate(urg_serial_t *serial, long baudrate);


//! データを送信する
extern int serial_write(urg_serial_t *serial, const char *data, int size);


//! データを受信する
extern int serial_read(urg_serial_t *serial,
                       char *data, int max_size, int timeout);


//! 改行までのデータを受信する
extern int serial_readline(urg_serial_t *serial,
                           char *data, int max_size, int timeout);


//! エラー文字列を格納して返す
extern int serial_error(urg_serial_t *serial,
                        char *error_message, int max_size);

#ifdef __cplusplus
}
#endif

#endif /* !URG_SERIAL_H */
