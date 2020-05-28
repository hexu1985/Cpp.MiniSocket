#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <thread>

#include "config.hpp"
#include "err_quit.hpp"

int main(int argc, char **argv)
{
    SOCKET				listenfd, connfd;
    socklen_t			clilen;
    struct sockaddr_in	cliaddr, servaddr;
    unsigned short		port = SERV_PORT;
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
    servaddr.sin_port        = htons(port);	/* daytime server */

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR)
        err_quit("bind error");

    if (listen(listenfd, 1) == SOCKET_ERROR)
        err_quit("listen error");

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    WSACleanup();
}

