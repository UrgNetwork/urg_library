#ifndef TINY_CUNIT_H
#define TINY_CUNIT_H

/*!
  \file
  \brief テスト用 assert() 関数群

  \author Satofumi KAMIMURA

  $Id$
*/


void test_begin(const char* test_name);
void test_end(void);
void test_assert_equal_int(const long expected, const long actual,
                           const char* src_file, int line_no);
void test_assert_equal_char(const char expected, const char actual,
                            const char* src_file, int line_no);

#define TEST_ASSERT_EQUAL_INT(A, B) test_assert_equal_int(A, B, __FILE__, __LINE__)
#define TEST_ASSERT_EQUAL_CHAR(A, B) test_assert_equal_char(A, B, __FILE__, __LINE__)

#endif /* !TINY_CUNIT_H */
