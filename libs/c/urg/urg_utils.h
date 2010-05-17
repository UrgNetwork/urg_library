#ifndef URG_UTILS_H
#define URG_UTILS_H

/*!
  \file
  \brief URG センサ用の補助関数

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"


/*!
  \brief URG のエラーを示す文字列を返す

  \param[in] urg URG センサ管理

  \retval URG のエラーを示す文字列

  Example
  \code
  if (!urg_open("/dev/ttyACM0", 115200)) {
      printf("urg_connect: %s\n", urg_error(&urg));
      return -1;
  } \endcode
*/
extern const char *urg_error(urg_t *urg);


// !!!
extern int urg_width_min(urg_t *urg);


// !!!
extern int urg_width_max(urg_t *urg);


//! 取得できるデータ数の最大数を返す
extern int urg_data_max(urg_t *urg);
// !!! データ取得に必要な配列のサイズを返す関数を、ここで用意する


// !!! ID を返す
// !!! ID を使って、接続するセンサを選択するサンプルを作る


// !!! パラメータを返す


// !!! バージョンを返す


// !!! II の故障情報を返す


// !!! デバイス一覧を返す
//extern urg_


// !!! URG 用 USB ドライバを用いているデバイス一覧を返す


#endif /* !URG_UTILS_H */
