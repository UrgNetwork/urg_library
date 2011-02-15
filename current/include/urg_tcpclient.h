#ifndef URG_TCPCLIENT_H
#define URG_TCPCLIENT_H

/*!
  \file
  \brief TCP/IP read/write functions

  \author Katsumi Kimoto

  $Id$
*/

#include "urg_ring_buffer.h"
#include "urg_detect_os.h"
#include <sys/types.h>
#if defined(URG_WINDOWS_OS)
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
    struct sockaddr_in server_addr;
    int sock_desc;
    int sock_addr_size;

    // buffer
    ring_buffer_t rb;
    char buf[RB_SIZE];

    // line reading functions
    int pushed_back; // for pushded back char

} urg_tcpclient_t;
// -- end of NON INTERFACE definitions --


// -- belows are MODULE INTERFACES --

/*!
  \brief constructor of tcp client module

  \param[out] cli tcp client type variable which must be allocated by a caller.
  \param[in] server_ip_str IP address expressed in string, i.e. "192.168.0.1"
  \param[in] port_num port number expressed in integer, i.e. port_num = 10200

  \retval 0 succeeded.
  \retval -1 error
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
int tcpclient_read(urg_tcpclient_t* cli, char* userbuf, int req_size,
                   int timeout);


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
int tcpclient_readline(urg_tcpclient_t* cli, char* userbuf, int buf_size,
                       int timeout);

#endif /* !URG_TCPCLIENT_H */
