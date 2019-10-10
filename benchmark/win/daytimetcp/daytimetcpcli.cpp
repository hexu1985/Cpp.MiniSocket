#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.hpp"
#include "err_quit.hpp"

int main(int argc, char *argv[])
{
    SOCKET              sockfd;
    int					n;
    char				recvline[MAXLINE + 1];
    struct sockaddr_in	servaddr;
    unsigned short      port = 13;
    WSADATA             wsadata;

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
    servaddr.sin_port   = htons(port);	/* daytime server */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR)
        err_quit("connect error");

    while ( (n = recv(sockfd, recvline, MAXLINE, 0)) > 0) {
        recvline[n] = 0;	/* null terminate */
        if (fputs(recvline, stdout) == EOF)
            err_quit("fputs error");
    }
    if (n == SOCKET_ERROR)
        err_quit("read error");

    WSACleanup();
    exit(0);
}
