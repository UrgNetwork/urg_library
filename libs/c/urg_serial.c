/*!
  \file
  \brief シリアル通信

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_serial.h"

#include <stdio.h>


enum {
    False = 0,
    True,
};


#if defined(URG_WINDOWS_OS)
#include "urg_serial_windows.c"
#else
#include "urg_serial_linux.c"
#endif


// 改行かどうかの判定
static int isLF(const char ch)
{
    return ((ch == '\r') || (ch == '\n')) ? 1 : 0;
}


static void serial_ungetc(serial_t *serial, char ch)
{
    serial->has_last_ch = True;
    serial->last_ch = ch;
}


int serial_readline(serial_t *serial, char *data, int max_size, int timeout)
{
    /* １文字ずつ読み出して評価する */
    int filled = 0;
    int is_timeout = 0;

    while (filled < max_size) {
        char recv_ch;
        int n = serial_read(serial, &recv_ch, 1, timeout);
        if (n <= 0) {
            is_timeout = 1;
            break;
        } else if (isLF(recv_ch)) {
            fprintf(stderr, "\n");
            break;
        }
        data[filled++] = recv_ch;
        fprintf(stderr, "%c", recv_ch);
    }
    if (filled == max_size) {
        --filled;
        serial_ungetc(serial, data[filled]);
    }
    data[filled] = '\0';

    if ((filled == 0) && is_timeout) {
        return -1;
    } else {
        return filled;
    }
}
