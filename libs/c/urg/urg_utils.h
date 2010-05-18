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
      printf("urg_open: %s\n", urg_error(&urg));
      return -1;
  } \endcode
*/
extern const char *urg_error(const urg_t *urg);


// !!! 距離の最小値
extern long urg_distance_min(const urg_t *urg);


// !!! 距離の最大値
extern long urg_distance_max(const urg_t *urg);

// !!!
extern int urg_width_min(const urg_t *urg);


// !!!
extern int urg_width_max(const urg_t *urg);


// !!! １スキャンにかかる時間 [msec]
extern int urg_scan_msec(const urg_t *urg);


//! 取得できるデータ数の最大数を返す
extern int urg_data_max(const urg_t *urg);


// !!! 最後に取得した設定において、角度とインデックスの変換を行う
extern double urg_index2rad(const urg_t *urg, int index);


// !!!
extern int urg_rad2index(const urg_t *urg, double radian);


// !!! ID を返す
extern const char *urg_sensor_id(const urg_t *urg);
// !!! ID を使って、接続するセンサを選択するサンプルを作る


// !!! バージョンを返す
extern const char *urg_sensor_version(const urg_t *urg);


// !!! II の故障情報を返す
extern const char *urg_sensor_status(const urg_t *urg);


// !!! デバイス一覧を返す
//extern urg_


// !!! URG 用 USB ドライバを用いているデバイス一覧を返す


#endif /* !URG_UTILS_H */
