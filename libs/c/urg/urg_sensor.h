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


//! urg_t 構造体の初期化
extern void urg_initialize(urg_t *urg);


/*!
  \brief 接続

  指定したデバイスに接続し、距離を計測できるようにします。

  \param[i/o] urg URG センサ管理
  \param[i] device 接続デバイス名
  \param[i] baudrate 接続ボーレート [bps]

  retval 0 正常
  retval <0 エラー

  Example
  \code
  urg_t urg;
  urg_initialize(&urg);

  if (!urg_open("/dev/ttyACM0", 115200)) {
    printf("urg_connect: %s\n", urg_get_error(&urg));
    return -1;
  } \endcode
*/
extern int urg_open(urg_t *urg, const char *device, long baudrate);


/*!
  \brief 切断

  レーザを消灯し、URG との接続を切断します。

  \param[i/o] urg URG センサ管理

  Example
  \code
  urg_close(&urg); \endcode
*/
extern void urg_close(urg_t *urg);


// !!! タイムスタンプモードの開始
// !!!
extern int urg_start_timestamp_mode(urg_t *urg);


// !!! タイムスタンプの取得
// !!!
extern long urg_get_timestamp(urg_t *urg);


// !!! タイムスタンプモードの終了
// !!!
extern void urg_stop_timestamp_mode(urg_t *urg);


// !!! データ取得に必要な配列のサイズを返す関数を、ここで用意するか
// !!! 拡張ライブラリで用意するかは未定
// !!!
// !!! 拡張ライブラリで用意することにする


// !!!
// !!! 案: urg_once_scan()
// !!! 1 スキャンのデータを取得します。
// !!! これはブロッキング関数です。データの受信が終わるまで、次の関数に処理が移りません。
// !!! サンプルコードを示す
// !!! urg_once_scan()
// !!! if (n == !!!) // 計測が完了してから、この行が処理される
extern int urg_get_distance(urg_t *urg, long data[], long *timestamp);


// !!!
// !!! 案: urg_scan_request()
// !!! データ計測の開始を指示します。
extern int urg_start_measurement(urg_t *urg, int scan_times, int skip_scan);
extern int urg_receive_measurement(urg_t *urg, long data[], long *timestamp);


extern int urg_start_intensity_measurement(urg_t *urg,
                                           int scan_times, int skip_scan);
extern int urg_receive_intensity_measurement(urg_t *urg,
                                             long data[], long *timestamp);


extern int urg_start_multiecho_measurement(urg_t *urg,
                                           int scan_times, int skip_scan);
extern int urg_receive_multiecho_measurement(urg_t *urg,
                                             long data[], long *timestamp);


// !!! 案: urg_scan_stop()
// !!! urg_scan_request() で行った計測を途中で中止したい場合に用いる。
extern void urg_stop_measurement(urg_t *urg);


// !!!
// !!! 案: (first_step, last_step, culster_step
// !!! cluster_step については、十分に説明する
extern int urg_set_width(urg_t *urg,
                         int first_step, int last_step, int skip_step);


// !!!
// !!! レーザを発光させる
extern int urg_laser_on(urg_t *urg);


// !!!
// !!! レーザを消灯する
extern void urg_laser_off(urg_t *urg);

#endif /* !URG_SENSOR_H */
