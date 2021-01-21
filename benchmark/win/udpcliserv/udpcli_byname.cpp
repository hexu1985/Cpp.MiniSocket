#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "dg_cli.hpp"
#include "udp_connect.hpp"

int main(int argc, char **argv)
{
    SOCKET  sockfd;
    WSADATA wsadata;

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    if (argc != 3)
        err_quit("usage: daytimetcpcli <hostname/IPaddress> <service/port#>");

    sockfd = udp_connect(argv[1], argv[2]);

    dg_cli(stdin, sockfd, NULL, 0);

    exit(0);
}
