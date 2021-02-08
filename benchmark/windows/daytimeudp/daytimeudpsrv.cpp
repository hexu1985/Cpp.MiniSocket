#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "sock_ntop.hpp"

int main(int argc, char **argv)
{
    SOCKET             sockfd;
    int                n;
    char               buff[MAXLINE];
    time_t             ticks;
    socklen_t          len;
    struct sockaddr_in servaddr, cliaddr;
    unsigned short     port = SERV_PORT;
    WSADATA            wsadata;

    if (argc != 1 && argc != 2)
        err_quit("usage: a.out [listen_port]");

    if (argc == 2)
        port = atoi(argv[1]);

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
        err_quit("usage: a.out [listen_port]");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR)
        err_quit("bind error");

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
