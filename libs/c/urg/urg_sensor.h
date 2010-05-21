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
    URG_COMMUNICATION_3_BYTE,           //!< 距離を 3 byte で表現する
    URG_COMMUNICATION_2_BYTE,           //!< 距離を 2 byte で表現する
} range_byte_t;


enum {
    URG_SCAN_INFINITY = 0,      //!< 無限回のデータ取得
};


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

  if (urg_open(&urg, "/dev/ttyACM0", 115200, URG_SERIAL) < 0) {
      return 1;
  }

  ...

  urg_close(&urg); \endcode

  \see urg_close()
*/
extern int urg_open(urg_t *urg, const char *device, long baudrate_or_port,
                    connection_type_t connection_type);


/*!
  \brief 切断

  レーザを消灯し、URG との接続を切断します。

  \param[in,out] urg URG センサ管理

  \see urg_open()
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

  before_ticks = urg_ticks();
  timestamp = urg_timestamp();
  after_ticks = urg_ticks();

  // タイムスタンプについての計算
  ...

  urg_stop_timestamp_mode(&urg); \endcode

  詳しくは \ref sync_timestamp.c を参照して下さい。
*/
extern long urg_timestamp(urg_t *urg);


//! タイムスタンプモードの終了
extern void urg_stop_timestamp_mode(urg_t *urg);


/*!
  \brief 距離データの取得を開始

  距離データの取得を開始します。実際のデータは get_distance(), get_distance_intensity(), get_multiecho() で取得できます。

  \param[in,out] urg URG センサ管理
  \param[in] type データ・タイプ
  \param[in] scan_times データの取得回数
  \param[in] skip_scan データの取得間隔

  \retval 0 正常
  \retval <0 エラー

  type は !!!

  - URG_DISTANCE
  - URG_DISTANCE_INTENSITY
  - URG_MULTIECHO

  scan_times は !!!

  - URG_SCAN_INFINITY

  skip_scan は !!!

  !!! skip_scan についての図

  Example
  \code
  enum { CAPTURE_TIMES = 10 };
  start_measurement(&urg, URG_DISTANCE, CAPTURE_TIMES, 0);

  for (i = 0; i < CAPTURE_TIMES; ++i) {
      n = urg_get_distance(&urg, data, &timestamp);

      // 受信したデータの利用
      ...
  } \endcode

  \see urg_get_distance(), urg_get_distance_intensity(), urg_get_multiecho(), urg_stop_measurement()
*/
extern void urg_start_measurement(urg_t *urg, measurement_type_t type,
                                  int scan_times, int skip_scan);


/*!
  \brief 距離データの取得

  !!!
*/
extern void urg_get_distance(urg_t *urg, long data[], long *timestamp);


/*!
  \brief 距離と強度データの取得

  !!!
*/
extern void urg_get_distance_intensity(urg_t *urg, long data[],
                                       unsigned short intensity[],
                                       long *timestamp);


/*!
  \brief マルチエコー・データの取得

  マルチエコーとは、!!!

  強度データも含まれます。

  !!!
*/
extern void urg_get_multiecho(urg_t *urg, long data_multi[],
                              unsigned short intensity_multi[],
                              long *timestamp);


/*!
  \brief 計測を中断し、レーザを消灯させます

  \ref urg_start_measurement() の計測を中断します。

  Example
  \code
  !!!
  \endcode

  \see urg_get_distance(), urg_get_distance_intensity(), urg_get_multiecho()
*/
extern void urg_stop_measurement(urg_t *urg);


/*!
  \brief 計測範囲を設定します

  !!!
*/
extern int urg_set_scaning_parameter(urg_t *urg, int first_step, int last_step,
                                     int skip_step);


/*!
  \brief 通信データのサイズ変更

  !!! 距離データ受信の際のデータサイズを変更します。

  この設定は \ref urg_start_measurement() で type を URG_DISTANCE に指定したときのみ有効です。設定は、次の \ref urg_start_measurement() から有効になります。

  !!! データサイズを小さくできますが、計測できる距離は !!!! [mm] 以下になります。!!! よりも遠い位置の距離を検出した場合、返される値は !!!! になります。

  !!! 通信速度が遅い場合は、設定を URG_COMMUNICATION_2_BYTE にすることで urg_get_distance() の応答性が改善される場合があります。

  初期状態では URG_COMMUNICATION_3_BYTE が設定されています。
*/
extern int urg_set_communication_data_size(range_byte_t range_byte);


//! レーザを発光させる
extern int urg_laser_on(urg_t *urg);


//! レーザを消灯する
extern void urg_laser_off(urg_t *urg);

#endif /* !URG_SENSOR_H */
