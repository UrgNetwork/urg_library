#include "urg_ring_buffer.h"
#include "urg_detect_os.h"
#include <sys/types.h>
#if defined(URG_WINDOWS_OS)
//#include <winsock2.h>
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

typedef struct {
    // socket
    struct sockaddr_in server_addr; /* socket */
    int sock_desc;                  /* socket file descriptor */
    int sock_addr_size;

    // buffer
    ring_buffer_t rb;  // ring buffer
    char buf[RB_SIZE];

    // line reading functions
    int pushed_back; // for pushded back char

} urg_tcpclient_t;
// -- end of NON INTERFACE definitions --


// -- belows are MODULE INTERFACES --

/* constructor of tcp client module
 * cli          : tcp client type variable which must be allocated by a caller.
 * server_ip_str: IP address expressed in string, i.e. "192.168.0.1"
 * port_num     : port number expressed in integer, i.e. port_num = 10200
 * returns -1 when error, 0 when succeeded.
 */
int tcpclient_open(urg_tcpclient_t* cli,
                   const char* server_ip_str, int port_num);

/* destructor of tcp client module
 * cli : tcp client type variable which must be deallocated by a caller
 *       after closing.
 */
void tcpclient_close(urg_tcpclient_t* tcpc);

/* read from socket.
 * userbuf : buffer to store read data which must be allocated by a caller.
 * req_size: data size requested to read in byte.
 * timeout : time out specification which unit is microsecond.
 * returns the number of data read, -1 when error.
 */
int tcpclient_read(urg_tcpclient_t* cli, char* userbuf, int req_size, int timeout);

/* write to socket.
 * userbuf : data to write.
 * req_size: data size requested to write in byte.
 * returns the number of data wrote, -1 when error.
 */
int tcpclient_write(urg_tcpclient_t* cli, const char* userbuf, int req_size);

// not implemented yet.
int tcpclient_error( urg_tcpclient_t* cli, char* errror_message, int max);

/* read one line from socket.
 * userbuf : buffer to store read data which must be allocated by a caller.
 * req_size: data size requested to read in byte.
 * timeout : time out specification which unit is microsecond.
 * returns the number of data read, -1 when error.
 */
int tcpclient_readline(urg_tcpclient_t* cli, char* userbuf, int buf_size, int timeout);
