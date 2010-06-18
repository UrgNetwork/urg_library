/*!
  \brief urg_sensor.c のテスト

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.c"
#include "tiny_cunit.h"


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    test_begin("checksum() test");
    TEST_ASSERT_EQUAL_CHAR('P', checksum("00", 2));
    test_end();

    // !!!

    return 0;
}
