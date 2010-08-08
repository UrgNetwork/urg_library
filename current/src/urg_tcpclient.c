#include "urg_detect_os.h"

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* malloc() */
#include <unistd.h> /* close() */

#include <string.h>
#include <strings.h>
#include <sys/time.h> /* timeval */

#if defined(URG_WINDOWS_OS)
//#pragma comment(lib, "wininet.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

#include "urg_tcpclient.h"

static void tcpclient_buffer_init( urg_tcpclient_t* cli )
{
  ring_initialize( &cli->rb, cli->buf, RB_BITSHIFT );
}

// get number of data in buffer.
static int tcpclient_buffer_data_num( urg_tcpclient_t* cli )
{
  return ring_size( &cli->rb );
}

static int tcpclient_buffer_write( urg_tcpclient_t* cli, const char* data, int size )
{
  int ret;
  ret = ring_write( &cli->rb, data, size );
  //  {int rem = ring_size(&cli->rb);
  //    printf("\n(BUFW:rem%d:%d:", rem,size);{int i;for(i=0; i<size; i++)putchar(data[i]);}printf(")\n");
  //  }
  return ret;
}

static int tcpclient_buffer_read( urg_tcpclient_t* cli, char* data, int size )
{
  int ret = ring_read( &cli->rb, data, size );
  //{int rem = ring_size(&cli->rb);
  //    printf("\n(BUFR:rem%d:%d:", rem,size);{int i;for(i=0; i<size; i++)putchar(data[i]);}printf(")\n");
  //  }
  return ret;
}


int tcpclient_open(urg_tcpclient_t* cli, const char* ip_str, int port_num)
{
  cli->pushed_back = -1; // no pushed back char.

#if defined(URG_WINDOWS_OS)
  {
    WORD wVersionRequested = 0x0202;
    WSADATA WSAData;
    int err;
    err = WSAStartup(wVersionRequested, &WSAData);
    if ( err != 0 ) {
      printf("WSAStartup failed with error: %d\n", err);
      return -1;
    }
  }
#endif

  tcpclient_buffer_init( cli );

  cli->sock_addr_size = sizeof (struct sockaddr_in);

  if ( (cli->sock_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(-1);
  }

  memset( (char*)&(cli->server_addr), 0, sizeof(cli->sock_addr_size) );
  cli->server_addr.sin_family      = AF_INET;
  cli->server_addr.sin_port        = htons(port_num);

  /* bind is not required, and port number is dynamic */

  /*
  if ( cli->server_addr.sin_addr.s_addr  = hostGetByName(ip_str)) < 0 )
  */

  if ( (cli->server_addr.sin_addr.s_addr = inet_addr(ip_str)) == INADDR_NONE ) {
    perror("unknown server name");
    return -1;
  }

  if ( connect(cli->sock_desc, (const struct sockaddr *) &(cli->server_addr),
                cli->sock_addr_size) < 0 ) {
    perror("connect");
    close(cli->sock_desc);
    return -1;
  }

  //fprintf(stderr, "tcpclient_open() ... O.K.\n");
  return 0;
}


void tcpclient_close(urg_tcpclient_t* cli)
{
  close( cli->sock_desc);
}

int tcpclient_read(urg_tcpclient_t* cli, char* userbuf, int req_size, int timeout)
{
  
  // number of data in buffer.
  int num_in_buf = tcpclient_buffer_data_num(cli); 
  int sock       = cli->sock_desc;
  int rem_size   = req_size;  // remaining size to be sent back.
  int n;

  // copy data in buffer to user buffer and return with requested size.
  //fprintf(stderr, "num_in_buf: %d\n", num_in_buf);
  if ( 0 < num_in_buf ) {
    n = tcpclient_buffer_read( cli, userbuf, req_size );
    // n never be greater than req_size
    rem_size = req_size - n;  // lacking size.
    if ( rem_size <= 0 ) {
      return req_size;
    }

    num_in_buf = tcpclient_buffer_data_num(cli); 
  }

  // data in buffer was not enough, read from socket to fill buffer,
  // without blocking, i.e. read from system's buffer.
  {
    char tmpbuf[ BUFSIZE ];
    // receive with non-blocking mode.
#if defined(URG_WINDOWS_OS)
    int no_timeout = 1;
    setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&no_timeout, sizeof(struct timeval) );
    n = recv(sock, tmpbuf, BUFSIZE - num_in_buf, 0);
#else
    n = recv(sock, tmpbuf, BUFSIZE - num_in_buf, MSG_DONTWAIT);
#endif
    //{int i;printf("(SOCK!%d!",n);for(i=0;i<n;i++)putchar(tmpbuf[i]);printf(")\n");}
    if ( 0 < n ) {
      tcpclient_buffer_write( cli, tmpbuf, n ); // copy socket to my buffer
      //printf("(mybuf-n=%d)",n);
    }

    n = tcpclient_buffer_read( cli, & userbuf[req_size-rem_size], rem_size );
    // n never be greater than rem_size
    rem_size -= n;
    if ( rem_size <= 0 ) {
      return req_size;
    }
  }

  //  lastly recv with blocking but with time out to read necessary size.
  {
#if defined(URG_WINDOWS_OS)
    setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(struct timeval) );
#else
    struct timeval tv;
    tv.tv_sec = timeout/1000; // millisecond to seccond
    tv.tv_usec = (timeout % 1000) * 1000; // millisecond to microsecond
    setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval) );
#endif
    n = recv(sock, & userbuf[req_size-rem_size], rem_size, 0); //4th arg 0:no flag
    //{int i;printf("(SOCK2!%d!",n);for(i=0;i<n;i++)putchar(userbuf[req_size-rem_size+i]);printf(")\n");}
    // n never be greater than rem_size
    if ( 0 < n ) rem_size -= n;
  }

  return (req_size - rem_size); // last return may be less than req_size;
}

int tcpclient_write(urg_tcpclient_t* cli, const char* buf, int size)
{
  // blocking if data size is larger than system's buffer.
  return send(cli->sock_desc, buf, size, 0);  //4th arg 0: no flag
}

int tcpclient_readline(urg_tcpclient_t* cli, char* userbuf, int buf_size, int timeout)
{
  int n;
  int i = 0;
  if ( 0 < cli->pushed_back ) {
    userbuf[i] = cli->pushed_back;
    i++;
    cli->pushed_back = -1;
  }
  for ( ; i<buf_size; i++) {
    char ch;
    //fprintf(stderr, "tcpclient_read(): ");
    n = tcpclient_read( cli, &ch, 1, timeout);
    //fprintf(stderr, "n = %d\n", n);
    if ( n <= 0 ) {
      break; // error
    }
    if ( ch=='\n' || ch=='\r' ) {
      break; // success
    }
    //fprintf(stderr, "%c", ch);
    userbuf[i] = ch;
  }

  if ( buf_size <= i ) { // No CR or LF found.
    --i;
    cli->pushed_back = userbuf[buf_size-1] & 0xff;
    userbuf[buf_size-1] = '\0';
  }

  userbuf[i] = '\0';

  if ( i==0 && n <= 0 ) { // error
    return -1;
  }

  //printf("TCP:%d:%s\n", i, userbuf);
  return i; // the number of characters filled into user buffer.
}

//int tcpclient_error( urg_tcpclient_t* cli, char* error_message, int max)
//{
//  error_message[0] = '\0';
//  return 0;
//}
