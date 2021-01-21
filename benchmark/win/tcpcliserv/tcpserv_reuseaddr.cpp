#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <thread>

#include "config.hpp"
#include "err_quit.hpp"
#include "str_echo.hpp"

static void doit(SOCKET sockfd);  /* each thread executes this function */

int main(int argc, char **argv)
{
    SOCKET             listenfd, connfd;
    socklen_t          clilen;
    struct sockaddr_in cliaddr, servaddr;
    unsigned short     port = SERV_PORT;
    const int          on = 1;
    WSADATA            wsadata;

    if (argc != 1 && argc != 2)
        err_quit("usage: a.out [listen_port]");

    if (argc == 2)
        port = atoi(argv[1]);

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        err_quit("socket error");

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on)) == SOCKET_ERROR)
        err_quit("setsockopt error");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port); /* daytime server */

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR)
        err_quit("bind error");

    if (listen(listenfd, LISTENQ) == SOCKET_ERROR)
        err_quit("listen error");

    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) == INVALID_SOCKET)
            err_quit("accept error");

        std::thread(doit, connfd).detach();
    }

    WSACleanup();
}

static void doit(SOCKET sockfd)
{
    str_echo(sockfd);         /* process the request */
    closesocket(sockfd);  /* done with connected socket */
}
