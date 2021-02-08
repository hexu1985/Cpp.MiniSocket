#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "dg_echo.hpp"
#include "udp_server.hpp"

int main(int argc, char **argv)
{
    SOCKET             sockfd;
    struct sockaddr_in cliaddr;
    WSADATA            wsadata;

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    if (argc == 2)
        sockfd = udp_server(NULL, argv[1], NULL);
    else if (argc == 3)
        sockfd = udp_server(argv[1], argv[2], NULL);
    else
        err_quit("usage: daytimetcpsrv4 [ <host> ] <service or port>");

    if (sockfd == INVALID_SOCKET)
        err_quit("udp_server error");

    dg_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}
