#ifndef URG_COMMUNICATION_H
#define URG_COMMUNICATION_H

/*!
  \file
  \brief 通信の処理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_serial.h"
#include "urg_ethernet_t.h"


/*! 定数定義 */
enum {
    COMMUNICATION_TIMEOUT = -1,    //!< タイムアウト発生エラー
};


/*! 通信リソースの管理 */
typedef struct
{
    serial_t serial;
    ethernet_t ethernet;

    // !!!
} communication_t;


/*! 通信タイプ */
typedef enum {
    URG_SERIAL,                 /*!< シリアル通信 */
    URG_ETHERNET,               /*!< イーサーネット通信 */
} communication_type_t;


/*!
  \brief 接続

  指定されたデバイスに接続する。

  \param[in,out] communication 通信リソースの管理
  \param[in] communication_type 接続タイプ
  \param[in] device 接続名
  \param[in] baudrate_or_port ボーレート / ポート番号

  \retval 0 正常
  \retval <0 エラー

  communication_type には

  - URG_SERIAL ... シリアル通信
  - URG_ETHERNET .. イーサーネット通信

  を指定する。

  device, baudrate_or_port の指定は communication_type により指定できる値が異なる。
  例えば、シリアル通信の場合は以下のようになる。

  Example
  \code
  communication_t communication;
  if (! communication_open(&communication, URG_SERIAL, "COM1", 115200)) {
  return 1;
  } \endcode

  また、イーサーネット通信の場合は以下のようになる。

  Example
  \code
  communication_t communication;
  if (! communication_open(&communication, URG_ETHERNET, "192.168.0.10", 10940)) {
  return 1;
  } \endcode

  \see communication_close()
*/
int communication_open(communication_t *communication,
                       communication_type_t communication_type,
                       const char *device, long baudrate_or_port);


/*!
  \brief 切断

  デバイスとの接続を切断する。

  \param[in,out] communication 通信リソースの管理

  \code
  communication_close(&communication); \endcode

  \see communication_open()
*/
void communication_close(communication_t *communication);


/*!
  \brief 送信

  データを送信する。

  \param[in,out] communication 通信リソースの管理
  \param[in] data 送信データ
  \param[in] size 送信バイト数

  \retval >=0 送信データ数
  \retval <0 エラー

  Example
  \code
  n = communication_write(&communication, "QT\n", 3); \endcode

  \see communication_read(), communication_readline()
*/
int communication_write(communication_t *communication,
                        const char *data, int size);


/*!
  \brief 受信

  データを受信する。

  \param[in,out] communication 通信リソースの管理
  \param[in] data 受信データを格納するバッファ
  \param[in] max_size 受信データを格納できるバイト数
  \param[in] timeout タイムアウト時間 [msec]

  \retval >=0 受信データ数
  \retval <0 エラー

  timeout に負の値を指定した場合、タイムアウトは発生しない。

  1 文字も受信しなかったときは #COMMUNICATION_TIMEOUT を返す。

  Example
  \code
  enum {
  BUFFER_SIZE = 256,
  TIMEOUT = 1000,           // [msec]
  };
  char buffer[BUFFER_SIZE];
  n = communication_read(&communication, buffer, BUFFER_SIZE, TIMEOUT); \endcode

  \see communication_write(), communication_readline()
*/
int communication_read(communication_t *communication,
                       char *data, int max_size, int timeout);


/*!
  \brief 改行文字までの受信

  改行文字までのデータを受信する。

  \param[in,out] communication 通信リソースの管理
  \param[in] data 受信データを格納するバッファ
  \param[in] max_size 受信データを格納できるバイト数
  \param[in] timeout タイムアウト時間 [msec]

  \retval >=0 受信データ数
  \retval <0 エラー

  data には、'\\0' 終端された文字列が max_size を越えないバイト数だけ格納される。 つまり、受信できる文字のバイト数は、最大で max_size - 1 となる。

  改行文字は '\\r' または '\\n' とする。

  受信した最初の文字が改行の場合は、0 を返し、1 文字も受信しなかったときは #COMMUNICATION_TIMEOUT を返す。

  \see communication_write(), communication_read()
*/
int communication_readline(communication_t *communication,
                           char *data, int max_size, int timeout);


#endif /* !URG_COMMUNICATION_H */
