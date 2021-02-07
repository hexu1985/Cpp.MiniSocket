#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "sock_ntop_host.hpp"

int connect_nonb(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec);
int tcp_connect_timeo(const char *host, const char *serv, int nsec);

int main(int argc, char **argv)
{
  int                     sockfd;
  socklen_t               len;
  struct sockaddr_storage ss;

  if (argc != 3) {
    fprintf(stderr, "usage: daytimetcpcli <hostname/IPaddress> <service/port#>\n");
    exit(1);
  }

  sockfd = tcp_connect_timeo(argv[1], argv[2], 3);
  if (sockfd < 0)
    err_quit("tcp_connect error");

  len = sizeof(ss);
  if (getpeername(sockfd, (struct sockaddr *)&ss, &len) < 0) {
    printf("connected to %s ok\n", argv[1]);
  } else {
    printf("connected to %s ok\n", sock_ntop_host((struct sockaddr *)&ss, len));
  }

  close(sockfd);

  return 0;
}

int connect_nonb(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
{
  int				flags, n, error;
  socklen_t		len;
  fd_set			rset, wset;
  struct timeval	tval;

  // 获取套接字flags
  if ((flags = fcntl(sockfd, F_GETFL, 0)) < 0) {
    return -1;
  }

  // 设置套接字为非阻塞的
  if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
    return -1;
  }

  error = 0;
  if ( (n = connect(sockfd, saptr, salen)) < 0)
    if (errno != EINPROGRESS)
      return(-1);

  /* Do whatever we want while the connect is taking place. */

  if (n == 0)
    goto done;	/* connect completed immediately */

  FD_ZERO(&rset);
  FD_SET(sockfd, &rset);
  wset = rset;
  tval.tv_sec = nsec;
  tval.tv_usec = 0;

  n = select(sockfd+1, &rset, &wset, NULL, nsec ? &tval : NULL);
  if (n == 0) {
    close(sockfd);		/* timeout */
    errno = ETIMEDOUT;
    return(-1);
  } else {
  }

  if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
    len = sizeof(error);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
      return(-1);			/* Solaris pending error */
  } else
    err_quit("select error: sockfd not set");

done:
  fcntl(sockfd, F_SETFL, flags);	/* restore file status flags */

  if (error) {
    close(sockfd);		/* just in case */
    errno = error;
    return(-1);
  }
  return(0);
}

int tcp_connect_timeo(const char *host, const char *serv, int nsec)
{
    int             sockfd, n;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        fprintf(stderr, "tcp_connect error for %s, %s: %s\n", host, serv, gai_strerror(n));
        return -1;
    }

    if (res == NULL) {
        fprintf(stderr, "tcp_connect error for %s, %s\n", host, serv);
        return -1;
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        fprintf(stderr, "tcp_connect error: socket error\n");
        freeaddrinfo(res);
        return -1;
    }

    if (connect_nonb(sockfd, res->ai_addr, res->ai_addrlen, nsec) < 0) {
        fprintf(stderr, "tcp_connect error: connect_nonb error for %s, %s\n", host, serv);
        perror("connect error");
        freeaddrinfo(res);
        return -1;
    } 

    freeaddrinfo(res);

    return (sockfd);
}
