#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "dg_cli.hpp"
#include "udp_connect.hpp"

int main(int argc, char **argv)
{
    int sockfd;

    if (argc != 3)
        err_quit("usage: daytimetcpcli <hostname/IPaddress> <service/port#>");

    sockfd = udp_connect(argv[1], argv[2]);

    dg_cli(stdin, sockfd, NULL, 0);

    exit(0);
}
