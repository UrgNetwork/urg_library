/*!
  \brief urg04lx_parameter_test.c のテスト

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "tiny_cunit.h"
#include <stdio.h>


int main(void)
{
    urg_t urg;
    int min_step;
    int max_step;
    long min_distance;
    long max_distance;


    if (urg_open(&urg, URG_SERIAL, "/dev/ttyACM0", 115200) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }

    test_begin("urg_step_min_max() test");
    urg_step_min_max(&urg, &min_step, &max_step);
    TEST_ASSERT_EQUAL_INT(-340, min_step);
    TEST_ASSERT_EQUAL_INT(+341, max_step);
    test_end();

    test_begin("urg_distance_min_max() test");
    urg_distance_min_max(&urg, &min_distance, &max_distance);
    TEST_ASSERT_EQUAL_INT(20, min_distance);
    TEST_ASSERT_EQUAL_INT(5600, max_distance);
    test_end();

    test_begin("urg_scan_usec() test");
    TEST_ASSERT_EQUAL_INT(100000, urg_scan_usec(&urg));
    test_end();

    test_begin("urg_max_index() test");
    TEST_ASSERT_EQUAL_INT(725, urg_max_index(&urg));
    test_end();

    urg_close(&urg);

    return 0;
}
