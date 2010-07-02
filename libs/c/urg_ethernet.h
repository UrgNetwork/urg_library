#ifndef URG_ETHERNET_H
#define URG_ETHERNET_H

/*!
  \file
  \brief イーサーネット通信

  \author Satofumi KAMIMURA

  $Id$
*/


typedef struct
{
    // !!!
} ethernet_t;


int ethernet_open(ethernet_t *ethernet, const char *address, long port);
void ethernet_close(ethernet_t *ethernet);
int ethernet_write(ethernet_t *ethernet, const char *data, int size);
int ethernet_read(ethernet_t *ethernet, char *data, int max_size, int timeout);
int ethernet_readline(ethernet_t *ethernet, char *data,
                      int max_size, int timeout);
int ethernet_error(ethernet_t *ethernet, char *error_message, int max_size);

#endif /* !URG_ETHERNET_H */
