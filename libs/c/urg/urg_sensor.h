#ifndef URG_SENSOR_H
#define URG_SENSOR_H

/*!
  \file
  \brief URG センサ制御

  URG 用の基本的な関数を提供します。

  \author Satofumi KAMIMURA

  $Id$
*/


//! URG センサ管理
typedef struct
{
    // !!! 共通のパラメータは、この構造体で管理する
    char dummy;
} urg_t;


//! 通信タイプ
typedef enum {
    URG_SERIAL,                 //!< シリアル通信
    URG_ETHERNET,               //!< イーサーネット通信
} connection_type_t;


//! 計測タイプ
typedef enum {
    URG_DISTANCE,               //!< 距離
    URG_DISTANCE_INTENSITY,     //!< 距離 + 強度
    URG_MULTIECHO,              //!< (距離 x 3) + (強度 x 3)
} measurement_type_t;


//! 距離を何 byte で表現するかの指定
typedef enum {
    URG_RANGE_2_BYTE,           //!< 距離を 2 byte で表現する
    URG_RANGE_3_BYTE,           //!< 距離を 3 byte で表現する
} range_byte_t;


enum {
    URG_SCAN_INFINITY = 0,      //!< 無限回のデータ取得
};


#if 0
/*!
  \brief URG 構造体の初期化

  urg_t を宣言したときは、URG ライブラリの関数を使う前にこの関数で初期化する必要があります。

  \param[in,out] urg URG センサ管理

  Example
  \code
  urg_t urg;
  urg_initialize(&urg);
  \endcode
*/
extern void urg_initialize(urg_t *urg);
#endif


/*!
  \brief 接続

  指定したデバイスに接続し、距離を計測できるようにします。

  \param[in,out] urg URG センサ管理
  \param[in] device 接続デバイス名
  \param[in] baudrate_or_port 接続ボーレート [bps] / TCP/IP ポート
  \param[in] connection_type 通信タイプ

  retval 0 正常
  retval <0 エラー

  Example
  \code
  urg_t urg;
  urg_initialize(&urg);

  if (urg_open(&urg, "/dev/ttyACM0", 115200, URG_SERIAL) < 0) {
      return 1;
  } \endcode
*/
extern int urg_open(urg_t *urg, const char *device, long baudrate_or_port,
                    connection_type_t connection_type);


/*!
  \brief 切断

  レーザを消灯し、URG との接続を切断します。

  \param[in,out] urg URG センサ管理

  Example
  \code
  urg_close(&urg); \endcode
*/
extern void urg_close(urg_t *urg);


//! タイムスタンプモードの開始
extern int urg_start_timestamp_mode(urg_t *urg);


/*!
  \brief タイムスタンプの取得

  \param[in,out] urg URG センサ管理

  \retval >=0 タイムスタンプ [msec]
  \retval <0 エラー

  Example
  \code
  urg_start_timestamp_mode(&urg);

  urg_ticks();
  timestamp = urg_timestamp();

  urg_stop_timestamp_mode(&urg);
  // !!!
  \endcode
*/
extern long urg_timestamp(urg_t *urg);


//! タイムスタンプモードの終了
extern void urg_stop_timestamp_mode(urg_t *urg);


#if 0
/*!
  \brief データの逐次取得

  １スキャンの距離データを取得します。取得が完了するまで、この関数は処理をブロックします。

  \param[in,out] urg URG センサ管理

  \retval >0 !!!
  \retval <=0 !!!

  Example
  \code
  // !!!
  \endcode
*/
extern int urg_get_distance(urg_t *urg, long data[], long *timestamp);
#endif


extern void start_measurement(urg_t *urg, measurement_type_t type,
                              int scan_times, int skip_scan);

extern void get_distance(urg_t *urg, long data[], long *timestamp);
extern void get_distance_intensity(urg_t *urg, long data[],
                                   unsigned short intensity[], long *timestamp);
extern void get_multiecho(urg_t *urg, long data_multi[],
                          unsigned short intensity_multi[], long *timestamp);

extern void stop_measurement(urg_t *urg);

#if 0
// !!!
// !!! 案: urg_scan_request()
// !!! データ計測の開始を指示します。
//  \param[in,out] urg URG センサ管理
/*!
  \brief !!!

  !!!

  Example
  \code
  // !!!
  \endcode
*/
extern int urg_start_measurement(urg_t *urg, int scan_times, int skip_scan);


//  \param[in,out] urg URG センサ管理
extern int urg_receive_measurement(urg_t *urg, long data[], long *timestamp);


//  \param[in,out] urg URG センサ管理
extern int urg_start_intensity_measurement(urg_t *urg,
                                           int scan_times, int skip_scan);
//  \param[in,out] urg URG センサ管理
extern int urg_receive_intensity_measurement(urg_t *urg,
                                             long data[], long *timestamp);


//  \param[in,out] urg URG センサ管理
extern int urg_start_multiecho_measurement(urg_t *urg,
                                           int scan_times, int skip_scan);
//  \param[in,out] urg URG センサ管理
extern int urg_receive_multiecho_measurement(urg_t *urg,
                                             long data[], long *timestamp);


/*!
  \brief 計測を中断し、レーザを消灯させます。

  urg_start_measurement(), urg_start_intensity_measurement(), urg_start_multiecho_measurement() の計測を中断するときに用います。

  \param[in,out] urg URG センサ管理

  Example
  \code
  // !!!
  \endcode

  \see urg_start_measurement(), urg_start_intensity_measurement(), urg_start_multiecho_measurement()
*/
extern void urg_stop_measurement(urg_t *urg);
#endif


// !!!
// !!! 案: (first_step, last_step, culster_step
// !!! cluster_step については、十分に説明する
extern int urg_set_width(urg_t *urg,
                         int first_step, int last_step, int skip_step);
extern int urg_set_range_width(urg_t *urg,
                               int first_step, int last_step, int skip_step);
extern int urg_set_scaning_parameter(urg_t *urg,
                                     int first_step, int last_step, int skip_step);


// !!!
// !!! デフォルトがどっちか、も書く
extern int urg_set_communication_data_size(range_byte_t range_byte);


//! レーザを発光させる
extern int urg_laser_on(urg_t *urg);


//! レーザを消灯する
extern void urg_laser_off(urg_t *urg);

#endif /* !URG_SENSOR_H */
