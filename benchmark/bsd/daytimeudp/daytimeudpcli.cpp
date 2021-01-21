#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.hpp"
#include "err_quit.hpp"

int main(int argc, char **argv)
{
    int                sockfd, n;
    char               recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    unsigned short     port = SERV_PORT;

    if (argc != 2 && argc != 3)
        err_quit("usage: udpcli <IPaddress> [port]");

    if (argc == 3)
        port = atoi(argv[2]);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        err_quit("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    sendto(sockfd, "", 1, 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));  /* send 1-byte datagram */

    n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, 0);
    recvline[n] = '\0';  /* null terminate */
    fputs(recvline, stdout);

    exit(0);
}
