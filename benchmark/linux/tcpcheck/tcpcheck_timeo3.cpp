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
#include "connect_nonb.hpp"
#include "sock_ntop_host.hpp"

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

int tcp_connect_timeo(const char *host, const char *serv, int nsec)
{
    int             sockfd, n;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct gaicb *reqs[1];
    struct gaicb req; 
    reqs[0] = &req;
    memset(reqs[0], 0, sizeof(*reqs[0]));

    reqs[0]->ar_name = host;
    reqs[0]->ar_service = serv;
    reqs[0]->ar_request = &hints;

    if ((n = getaddrinfo_a(GAI_NOWAIT, reqs, 1, NULL)) != 0) {
        fprintf(stderr, "getaddrinfo_a error for %s, %s: %s\n", host, serv, gai_strerror(n));
        return -1;
    }

    struct timespec timeout;
    timeout.tv_sec = nsec;
    timeout.tv_nsec = 0;

    if ((n = gai_suspend(reqs, 1, &timeout)) != 0) {
        fprintf(stderr, "gai_suspend(): %s\n", gai_strerror(n));
        return -1;
    }

    if ((n = gai_error(reqs[0])) != 0) {
        fprintf(stderr, "gai_suspend(): %s\n", gai_strerror(n));
        return -1;
    }

    res = reqs[0]->ar_result;
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
