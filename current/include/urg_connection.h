#ifndef URG_CONNECTION_H
#define URG_CONNECTION_H

/*!
  \file
  \brief 通信の処理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_serial.h"
#include "urg_tcpclient.h"


/*! 定数定義 */
enum {
    URG_CONNECTION_TIMEOUT = -1,
};


/*! 通信タイプ */
typedef enum {
    URG_SERIAL,
    URG_ETHERNET,
} urg_connection_type_t;


/*! 通信リソースの管理 */
typedef struct
{
    urg_connection_type_t type;
    urg_serial_t serial;
    urg_tcpclient_t tcpclient;
} urg_connection_t;


/*!
  \brief 接続

  指定されたデバイスに接続する。

  \param[in,out] connection 通信リソースの管理
  \param[in] connection_type 接続タイプ
  \param[in] device 接続名
  \param[in] baudrate_or_port ボーレート / ポート番号

  \retval 0 正常
  \retval <0 エラー

  connection_type には

  - URG_SERIAL ... シリアル通信
  - URG_ETHERNET .. イーサーネット通信

  を指定する。

  device, baudrate_or_port の指定は connection_type により指定できる値が異なる。
  例えば、シリアル通信の場合は以下のようになる。

  Example
  \code
  connection_t connection;
  if (! connection_open(&connection, URG_SERIAL, "COM1", 115200)) {
      return 1;
  } \endcode

  また、イーサーネット通信の場合は以下のようになる。

  Example
  \code
  connection_t connection;
  if (! connection_open(&connection, URG_ETHERNET, "192.168.0.10", 10940)) {
      return 1;
  } \endcode

  \see connection_close()
*/
extern int connection_open(urg_connection_t *connection,
                           urg_connection_type_t connection_type,
                           const char *device, long baudrate_or_port);


/*!
  \brief 切断

  デバイスとの接続を切断する。

  \param[in,out] connection 通信リソースの管理

  \code
  connection_close(&connection); \endcode

  \see connection_open()
*/
extern void connection_close(urg_connection_t *connection);


/*! ボーレートを設定する */
extern int connection_set_baudrate(urg_connection_t *connection, long baudrate);


/*!
  \brief 送信

  データを送信する。

  \param[in,out] connection 通信リソースの管理
  \param[in] data 送信データ
  \param[in] size 送信バイト数

  \retval >=0 送信データ数
  \retval <0 エラー

  Example
  \code
  n = connection_write(&connection, "QT\n", 3); \endcode

  \see connection_read(), connection_readline()
*/
extern int connection_write(urg_connection_t *connection,
                            const char *data, int size);


/*!
  \brief 受信

  データを受信する。

  \param[in,out] connection 通信リソースの管理
  \param[in] data 受信データを格納するバッファ
  \param[in] max_size 受信データを格納できるバイト数
  \param[in] timeout タイムアウト時間 [msec]

  \retval >=0 受信データ数
  \retval <0 エラー

  timeout に負の値を指定した場合、タイムアウトは発生しない。

  1 文字も受信しなかったときは #CONNECTION_TIMEOUT を返す。

  Example
  \code
enum {
    BUFFER_SIZE = 256,
    TIMEOUT_MSEC = 1000,
};
char buffer[BUFFER_SIZE];
n = connection_read(&connection, buffer, BUFFER_SIZE, TIMEOUT_MSEC); \endcode

  \see connection_write(), connection_readline()
*/
extern int connection_read(urg_connection_t *connection,
                           char *data, int max_size, int timeout);


/*!
  \brief 改行文字までの受信

  改行文字までのデータを受信する。

  \param[in,out] connection 通信リソースの管理
  \param[in] data 受信データを格納するバッファ
  \param[in] max_size 受信データを格納できるバイト数
  \param[in] timeout タイムアウト時間 [msec]

  \retval >=0 受信データ数
  \retval <0 エラー

  data には、'\\0' 終端された文字列が max_size を越えないバイト数だけ格納される。 つまり、受信できる文字のバイト数は、最大で max_size - 1 となる。

  改行文字は '\\r' または '\\n' とする。

  受信した最初の文字が改行の場合は、0 を返し、1 文字も受信しなかったときは #CONNECTION_TIMEOUT を返す。

  \see connection_write(), connection_read()
*/
extern int connection_readline(urg_connection_t *connection,
                               char *data, int max_size, int timeout);

#endif /* !URG_CONNECTION_H */
