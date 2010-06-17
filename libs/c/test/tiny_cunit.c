/*!
  \file
  \brief テスト用 assert() 関数群

  \author Satofumi KAMIMURA

  $Id$
*/

#include <stdio.h>
#include <stdlib.h>


void begin_test(const char* test_name)
{
    printf("%s ... ", test_name);
}


void end_test(void)
{
    printf("O.K.\n");
}


void assert_equal_char(const char expected, const char actual,
                       const char* src_file, int line_no)
{
    if (expected == actual) {
        return;
    }

    printf("fail!\n"
           "\n");

    printf("%s:%d:\n", src_file, line_no);
    printf("expected: %c\n"
           "actual  : %c\n", expected, actual);

    exit(1);
}
