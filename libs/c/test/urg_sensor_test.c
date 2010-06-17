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

    begin_test("checksum() test");
    ASSERT_EQUAL_CHAR('P', checksum("00", 2));
    end_test();

    // !!!

    return 0;
}
