/*!
  \brief urg_sensor.c のテスト

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.c"
#include "tiny_cunit.h"


int main(void)
{
    test_begin("checksum() test");
    TEST_ASSERT_EQUAL_CHAR('P', scip_checksum("00", 2));
    test_end();

    // !!!

    return 0;
}
