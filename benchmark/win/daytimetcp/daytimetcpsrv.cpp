#include <winsock2.h>
#include <ws2tcpip.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.hpp"
#include "err_quit.hpp"

int main(int argc, char *argv[])
{
    SOCKET              listenfd, connfd;
    struct sockaddr_in  servaddr;
    char                buff[MAXLINE];
    time_t              ticks;
    unsigned short      port = 13;
    WSADATA             wsadata;

    if (argc != 1 && argc != 2)
        err_quit("usage: a.out [listen_port]");

    if (argc == 2)
        port = atoi(argv[1]);

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        err_quit("socket error");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port); /* daytime server */

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR)
        err_quit("bind error");

    if (listen(listenfd, LISTENQ) == SOCKET_ERROR)
        err_quit("listen error");

    for ( ; ; ) {
        if ( (connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == INVALID_SOCKET)
            err_quit("accept error");

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        if (send(connfd, buff, strlen(buff), 0) == SOCKET_ERROR)
            err_quit("send error");

        closesocket(connfd);
    }

    WSACleanup();
}
