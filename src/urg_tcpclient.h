/* sockets */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "urg_ring_buffer.h"

// -- NOT INTERFACE, for internal use only --
enum { SOCK_ADDR_SIZE = sizeof(struct sockaddr_in) };
enum { COMMTCP_ERR = 1 };
enum { BUFSIZE_BITSHIFT = 8 }; /* bit shift is necessary
                                  for urg_ring_buffer I/F. */
enum { BUFSIZE = 1<<BUFSIZE_BITSHIFT };

typedef struct _tcpclient {
  // socket
  struct sockaddr_in server_addr; /* socket */
  int    sock_desc;		          /* socket file descriptor */
  int    sock_addr_size;

  // buffer
  ring_buffer_t rb;  // ring buffer
  char buf[BUFSIZE];

  // line reading functions
  int pushed_back; // for pushded back char

} tcpclient_t;
// -- end of NON INTERFACE definitions --


// -- belows are MODULE INTERFACES --

int tcpclient_open(tcpclient_t* tcpc,
                   const char* server_ip_str, int port_num);

void tcpclient_close(tcpclient_t* tcpc);

// Read from socket.
// userbuf : buffer to store read data.
// req_size: size requested to read.
// timeout : unit is microsecond.
// returns the number of data returned.
int tcpclient_read(tcpclient_t* cli, char* userbuf, int req_size, int timeout);
int tcpclient_write(tcpclient_t* cli, const char* userbuf, int req_size);
int tcpclient_error( tcpclient_t* cli, char* errror_message, int max);

int tcpclient_readline(tcpclient_t* cli, char* userbuf, int buf_size, int timeout);
