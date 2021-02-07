#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "tcp_connect_timeo.hpp"
#include "sock_ntop_host.hpp"

int main(int argc, char **argv)
{
    int                     sockfd;
    socklen_t               len;
    struct sockaddr_storage ss;

    if (argc != 3)
        err_quit("usage: daytimetcpcli <hostname/IPaddress> <service/port#>");

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
