#ifndef CONNECTION_H
#define CONNECTION_H

/*!
  \file
  \brief 通信の処理

  \author Satofumi KAMIMURA

  $Id$
*/


//! 通信用リソースの管理
typedef struct
{
    char dummy;
} connection_t;


//! 通信タイプ
typedef enum {
    URG_SERIAL,                 //!< シリアル通信
    URG_ETHERNET,               //!< イーサーネット通信
} connection_type_t;


int connection_open(connection_t *connection, connection_type_t connection_type,
                    const char *device, long baudrate);
void connection_close(connection_t *connection);

int connection_write(connection_t *connection, const char *data, int size);
int connection_read(connection_t *connection, char *data, int max_size);
// !!!


#endif /* !CONNECTION_H */
