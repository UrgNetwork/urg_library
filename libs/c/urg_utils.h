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


/*!
  \brief センサが返す距離の最大値、最小値を返す

  センサが返す距離を [最小値, 最大値] で返します。

  \param[in] urg URG センサ管理
  \param[out] min_distance 最小値 [mm]
  \param[out] max_distance 最大値 [mm]

  Example
  \code
  long min_distance, max_distance;
  urg_distance_min_max(&urg, &min_distance, &max_distance);

  for (int i = 0; i < n; ++i) {
  long distance = data[i];
  if ((distance < min_distance) || (distance > max_distance)) {
  continue;
  }
  ...
  } \endcode
*/
extern void urg_distance_min_max(const urg_t *urg,
                                 long *min_distance, long *max_distance);


/*!
  \brief 計測 step の最大値、最小値を返す

  urg_set_scanning_parameter() で指定できる範囲を [最小値, 最大値] で返す。

  \param[in] urg URG センサ管理
  \param[out] min_step 最小値
  \param[out] max_step 最大値

  step はセンサ正面が 0 であり、センサ上部から見た場合の反時計まわりの方向が正、時計まわりの方向が負の step 値となる。

  \image html sensor_step_image.jpg センサと step の関係

  min_step, max_step の値はセンサによって異なる。

  Example
  \code
  urg_step_min_max(&urg, &min_step, &max_step);

  printf("range first: %d [deg]\n", urg_step2deg(&urg, min_step));
  printf("range last : %d [deg]\n", urg_step2deg(&urg, max_step)); \endcode

  \see urg_set_scanning_parameter(), urg_step2rad(), urg_step2deg()
*/
extern void urg_step_min_max(const urg_t *urg, int *min_step, int *max_step);


/*! １スキャンにかかる時間 [usec] を返す */
extern long urg_scan_usec(const urg_t *urg);


/*! 取得データ数の最大値を返す */
extern int urg_max_index(const urg_t *urg);


/*!
  \brief インデックスと角度(radian)の変換を行う

  インデックとは urg_get_distance() などの距離データ取得関数が返したデータ配列についての値である。この関数は、最後に行った距離データ取得関数のデータ配列について有効となる。

  \param[in] urg URG センサ管理
  \param[in] index インデックス

  \return 角度 [radian]

  index は、取得した計測データについての値であり step や角度との関係は取得設定により異なる。

  \image html sensor_index_image.jpg センサの計測範囲とインデックスの関係

  Example
  \code
  int n = urg_get_distance(&urg, data, NULL);
  for (int i = 0; i < n; ++i) {
  long distance = data[i];
  double radian = urg_index2rad(i);
  double x = distance * cos(radian);
  double y = distance * sin(radian);
  printf("%.1f, %.1f\n", x, y);
  } \endcode

  \see urg_index2deg(), urg_rad2index(), urg_deg2index()
*/
extern double urg_index2rad(const urg_t *urg, int index);


/*! インデックスと角度(degree)の変換を行う */
extern double urg_index2deg(const urg_t *urg, int index);


/*! 角度(radian)とインデックスの変換を行う */
extern int urg_rad2index(const urg_t *urg, double radian);


/*! 角度(degree)とインデックスの変換を行う */
extern int urg_deg2index(const urg_t *urg, double degree);


/*!
  \brief 角度(radian)と step の変換を行う

  urg_step_min_max() で定義されている step について、角度(radian)と step の変換を行う。

  \param[in] urg URG センサ管理
  \param[in] radian 角度 [radian]

  \return step

  \image html sensor_angle_image.jpg センサの step と角度との関係

  角度から step へ変換した結果が整数でない場合、結果は 0 の方向に切り捨てられた値となる。

  \see urg_step_min_max(), urg_deg2step(), urg_step2rad(), urg_step2deg()
*/
extern int urg_rad2step(const urg_t *urg, double radian);


/*! 角度(degree)と step の変換を行う */
extern int urg_deg2step(const urg_t *urg, double degree);


/*! step と 角度(radian)の変換を行う */
extern double urg_step2rad(const urg_t *urg, int step);


/*! step と 角度(degree)の変換を行う */
extern double urg_step2deg(const urg_t *urg, int step);

/*! step とインデックスの変換を行う */
extern int urg_step2index(const urg_t *urg, int step);


/*!
  \brief センサのシリアル ID 文字列を返す

  センサのシリアル ID 文字列を返す。返される文字列はセンサ依存となる。

  \param[in] urg URG センサ管理

  \return シリアル ID 文字列
*/
extern const char *urg_sensor_id(const urg_t *urg);


/*!
  \brief センサのバージョン文字列を返す

  センサのソフトウェア・バージョン文字列を返す。返される文字列はセンサ依存となる。

  \param[in] urg URG センサ管理

  \return バージョン文字列
*/
extern const char *urg_sensor_version(const urg_t *urg);


/*!
  \brief センサのステータス文字列を返す

  センサのステータス文字列を返す。返される文字列はセンサ依存となる。

  \param[in] urg URG センサ管理

  \return ステータス文字列
*/
extern const char *urg_sensor_status(const urg_t *urg);


// !!!
extern int urg_find_port(char *port_name, int index);

#endif /* !URG_UTILS_H */
