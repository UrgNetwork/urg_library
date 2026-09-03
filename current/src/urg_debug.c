/*!
  \~japanese
  \brief URG センサ用の補助関数
  \~english Auxiliary functions (read and write raw data)
  \brief
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_debug.h"

int32_t urg_raw_write(urg_t *urg, const char *data, int32_t data_size)
{
    return connection_write(&urg->connection, data, data_size);
}

int32_t urg_raw_read(urg_t *urg, char *data, int32_t max_data_size, int32_t timeout)
{
    return connection_read(&urg->connection, data, max_data_size, timeout);
}

int32_t urg_raw_readline(urg_t *urg, char *data, int32_t max_data_size, int32_t timeout)
{
    return connection_readline(&urg->connection, data, max_data_size, timeout);
}
