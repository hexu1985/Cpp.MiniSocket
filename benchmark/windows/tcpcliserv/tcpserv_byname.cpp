#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <thread>

#include "config.hpp"
#include "err_quit.hpp"
#include "str_echo.hpp"
#include "tcp_listen.hpp"

static void doit(int sockfd);  /* each thread executes this function */

int main(int argc, char **argv)
{
    SOCKET           listenfd, connfd;
    socklen_t        addrlen, len;
    struct sockaddr *cliaddr;
    WSADATA          wsadata;

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    if (argc == 2)
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    else if (argc == 3)
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    else
        err_quit("usage: a.out [ <host> ] <service or port>");

    if (listenfd < 0)
        err_quit("tcp_listen error");

    cliaddr = (struct sockaddr *) malloc(addrlen);
    if (cliaddr == NULL)
        err_quit("malloc error");

    for ( ; ; ) {
        len = addrlen;
        if ( (connfd = accept(listenfd, cliaddr, &len)) < 0)
            err_quit("accept error");

        std::thread(doit, connfd).detach();
    }

    WSACleanup();
}

static void doit(int sockfd)
{
    str_echo(sockfd); /* process the request */
    closesocket(sockfd);  /* done with connected socket */
}
