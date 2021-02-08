#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "dg_echo.hpp"

int main(int argc, char **argv)
{
    SOCKET             sockfd;
    struct sockaddr_in servaddr, cliaddr;
    unsigned short     port = SERV_PORT;
    WSADATA            wsadata;

    if (argc != 1 && argc != 2)
        err_quit("usage: a.out [listen_port]");

    if (argc == 2)
        port = atoi(argv[1]);

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
        err_quit("usage: a.out [listen_port]");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))  == SOCKET_ERROR)
        err_quit("bind error");

    dg_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}
