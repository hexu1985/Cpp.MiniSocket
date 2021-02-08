#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <thread>

#include "config.hpp"
#include "err_quit.hpp"
#include "connect_timeo.hpp"

int main(int argc, char **argv)
{
    SOCKET             sockfd;
    struct sockaddr_in servaddr;
    unsigned short     port = SERV_PORT;
    WSADATA            wsadata;
    int                n;

    if (argc != 2 && argc != 3)
        err_quit("usage: a.out <IPaddress> [port]");

    if (argc == 3)
        port = atoi(argv[2]);

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        err_quit("socket error");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    n = connect_timeo(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr), 5);
    if (n < 0) {
        err_quit("connect error");
    } else if (n == 0) {
        err_quit("connect timeout");
    } else {
        std::cout << "connect to " << argv[1] << " : " << port << " ok!" << std::endl;
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    WSACleanup();
    exit(0);
}
