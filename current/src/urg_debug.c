/*!
  \~japanese
  \brief URG ƒZƒ“ƒT—p‚Ì•â•ŠÖ”

  \author Satofumi KAMIMURA

  $Id: urg_utils.c,v da778fd816c2 2011/01/05 20:02:06 Satofumi $
*/

#include "urg_debug.h"


int urg_raw_write(urg_t *urg, const char *data, int data_size)
{
    return connection_write(&urg->connection, data, data_size);
}


int urg_raw_read(urg_t *urg, char *data, int max_data_size, int timeout)
{
    return connection_read(&urg->connection, data, max_data_size, timeout);
}


int urg_raw_readline(urg_t *urg, char *data, int max_data_size, int timeout)
{
    return connection_readline(&urg->connection, data, max_data_size, timeout);
}
