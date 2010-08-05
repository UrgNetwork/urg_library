#include "urg_ring_buffer.h"
#include "urg_detect_os.h"
/* sockets */
#include <sys/types.h>
#if defined(URG_WINDOWS_OS)
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

// -- NOT INTERFACE, for internal use only --

enum { SOCK_ADDR_SIZE = sizeof(struct sockaddr_in) };

// For urg_ringbuffer.h
// The size of buffer must be specified by the power of 2
// i.e. ring buffer size = two to the RB_BITSHIFT-th power.
enum { RB_BITSHIFT = 8 };
enum { RB_SIZE     = 1<<RB_BITSHIFT };
// caution ! available buffer size is less than the
//           size given to the ring buffer(RB_SIZE).
enum { BUFSIZE     = RB_SIZE-1 };

typedef struct _tcpclient {
  // socket
  struct sockaddr_in server_addr; /* socket */
  int    sock_desc;		          /* socket file descriptor */
  int    sock_addr_size;

  // buffer
  ring_buffer_t rb;  // ring buffer
  char buf[RB_SIZE];

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
