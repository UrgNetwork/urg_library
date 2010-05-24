#ifndef SERIAL_UTILS_H
#define SERIAL_UTILS_H

/*!
  \file
  \brief シリアル用の補助関数

  \author Satofumi KAMIMURA

  $Id$
*/



// !!! port_name == NULL のときは、デバイスの数を返す
// !!! index 番目のデバイス名を port_name に格納する
// !!! index に特殊な値を指定したときは、デバイスの数を返す
// !!! バッファサイズは 64 byte 以上必要です
extern int serial_find_port(char *port_name, int index);

#endif /* !SERIAL_UTILS_H */
