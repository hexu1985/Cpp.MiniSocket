#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "udp_server.hpp"
#include "sock_ntop.hpp"

int main(int argc, char **argv)
{
    int                     sockfd;
    int                     n;
    char                    buff[MAXLINE];
    time_t                  ticks;
    socklen_t               len;
    struct sockaddr_storage cliaddr;
    WSADATA                 wsadata;

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    if (argc == 2)
        sockfd = udp_server(NULL, argv[1], NULL);
    else if (argc == 3)
        sockfd = udp_server(argv[1], argv[2], NULL);
    else
        err_quit("usage: daytimeudpsrv [ <host> ] <service or port>");

    if (sockfd < 0)
        err_quit("udp_server error");

    for ( ; ; ) {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, buff, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0)
            err_quit("recvfrom error");
        printf("datagram from %s\n", sock_ntop((struct sockaddr *)&cliaddr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        if (sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&cliaddr, len) != strlen(buff))
            err_quit("sendto error");
    }

    WSACleanup();
}
