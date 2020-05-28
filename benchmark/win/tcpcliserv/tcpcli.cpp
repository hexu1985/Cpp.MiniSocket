#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include "config.hpp"
#include "err_quit.hpp"
#include "str_cli.hpp"

int main(int argc, char **argv)
{
    SOCKET				sockfd;
    struct sockaddr_in	servaddr;
    unsigned short      port = SERV_PORT;
    WSADATA             wsadata;

    if (argc != 2 && argc != 3)
        err_quit("usage: a.out <IPaddress> [port]");

    if (argc == 3)
        port = atoi(argv[2]);

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        err_quit("socket error");

    unsigned long block = 1;
    std::cout << "block before ioctlsocket: " << block << std::endl;
    ioctlsocket((unsigned int)sockfd, FIONBIO, &block);
    std::cout << "block before after: " << block << std::endl;

    block = 0;
    std::cout << "block before ioctlsocket: " << block << std::endl;
    ioctlsocket((unsigned int)sockfd, FIONBIO, &block);
    std::cout << "block before after: " << block << std::endl;

#if 0
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR)
        err_quit("connect error");

    str_cli(stdin, sockfd);		/* do it all */
#endif
    closesocket(sockfd);

    WSACleanup();
    exit(0);
}
