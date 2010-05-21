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
  if (!urg_open(&urg, "/dev/ttyACM0", 115200, URG_SERIAL)) {
      printf("urg_open: %s\n", urg_error(&urg));
      return -1;
  } \endcode
*/
extern const char *urg_error(const urg_t *urg);


// !!! 距離の最小値
extern void urg_distance_min_max(const urg_t *urg,
                                 long *min_distance, long *max_distance);


// !!!
extern void urg_step_min_max(const urg_t *urg, int *min_index, int *max_index);


// !!! １スキャンにかかる時間 [msec]
extern int urg_scan_msec(const urg_t *urg);


//! 取得できるデータ数の最大数を返す
extern int urg_max_index(const urg_t *urg);


// !!! 最後に取得した設定において、角度とインデックスの変換を行う
extern double urg_index2rad(const urg_t *urg, int index);
extern int urg_index2deg(const urg_t *urg, int index);


// !!!
extern int urg_rad2step(const urg_t *urg, double radian);
extern int urg_deg2step(const urg_t *urg, int degree);


/*!
  \brief センサのシリアル ID 文字列を返す

  !!!

  !!! ID を使って、接続するセンサを選択するサンプルを作る
*/
extern const char *urg_sensor_id(const urg_t *urg);


/*!
  \brief センサのバージョン文字列を返す

  !!! バージョンを返す
*/
extern const char *urg_sensor_version(const urg_t *urg);


/*!
  \brief センサのステータス文字列を返す

  !!! II の故障情報を返す
*/
extern const char *urg_sensor_status(const urg_t *urg);


// !!! これらは別ファイルで行う
// COM デバイス一覧を返す
// URG 用 USB ドライバを用いているデバイス一覧を返す

#endif /* !URG_UTILS_H */
