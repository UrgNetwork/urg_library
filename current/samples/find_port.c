/*!
  \~japanese
  \example find_port.c ポートの探索

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_serial_utils.h"
#include <stdio.h>


int main(void)
{
    int found_port_size = serial_find_port();
    int i;

    if (found_port_size == 0) {
        printf("could not found ports.\n");
        return 1;
    }

    for (i = 0; i < found_port_size; ++i) {
        printf("%s\n", serial_port_name(i));
    }

    return 0;
}
