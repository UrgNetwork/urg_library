#ifndef URG_ETHERNET_H
#define URG_ETHERNET_H

/*!
  \file
  \brief イーサーネット通信

  \author Satofumi KAMIMURA

  $Id$
*/


// !!!
typedef struct
{
    // !!!
} ethernet_t;


extern int ethernet_open(ethernet_t *ethernet, const char *address, long port);
extern void ethernet_close(ethernet_t *ethernet);
extern int ethernet_write(ethernet_t *ethernet,
                          const char *data, int size);
extern int ethernet_read(ethernet_t *ethernet,
                         char *data, int max_size, int timeout);
extern int ethernet_readline(ethernet_t *ethernet, char *data,
                             int max_size, int timeout);
extern int ethernet_error(ethernet_t *ethernet,
                          char *error_message, int max_size);

#endif /* !URG_ETHERNET_H */
