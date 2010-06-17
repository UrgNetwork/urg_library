#ifndef TINY_CUNIT_H
#define TINY_CUNIT_H

/*!
  \file
  \brief テスト用 assert() 関数群

  \author Satofumi KAMIMURA

  $Id$
*/


void begin_test(const char* test_name);
void end_test(void);
void assert_equal_char(const char expected, const char actual,
                       const char* src_file, int line_no);

#define ASSERT_EQUAL_CHAR(A, B) assert_equal_char(A, B, __FILE__, __LINE__)

#endif /* !TINY_CUNIT_H */
