/*!
  \brief urg_serial.c のテスト

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_serial.h"
#include <stdio.h>


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    enum {
        BUFFER_SIZE = 128,
        TIMEOUT = 100,          // [msec]
    };

    serial_t serial;
    char buffer[BUFFER_SIZE];
    int n;

    if (serial_open(&serial, "/dev/ttyACM0", 19200) < 0) {
        // !!!
        perror("serial_open()");
        return 1;
    }

    serial_write(&serial, "VV\n", 3);

    while (1) {
        n = serial_readline(&serial, buffer, BUFFER_SIZE, TIMEOUT);
        if (n > 0) {
            printf("%s\n", buffer);
        } else {
            break;
        }
    }

    serial_close(&serial);

    return 0;
}
