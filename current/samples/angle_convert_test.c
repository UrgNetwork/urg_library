/*!
  \~japanese
  \example angle_convert_test.c 角度変換の結果を表示する

  \author Satofumi KAMIMURA

  $Id: sync_time_stamp.c,v 799c195d046c 2011/01/14 05:10:38 hokuyo $
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "open_urg_sensor.h"
#include <stdio.h>


int main(int argc, char *argv[])
{
    urg_t urg;
    int min_step;
    int max_step;

    if (open_urg_sensor(&urg, argc, argv) < 0) {
        return 1;
    }

    urg_step_min_max(&urg, &min_step, &max_step);

    printf("urg_step2deg(%d): %f\n", min_step, urg_step2deg(&urg, min_step));
    printf("urg_step2deg(%d): %f\n", max_step, urg_step2deg(&urg, max_step));

    printf("urg_step2rad(%d): %f\n", min_step, urg_step2rad(&urg, min_step));
    printf("urg_step2rad(%d): %f\n", max_step, urg_step2rad(&urg, max_step));

    return 0;
}
