#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "send_all.hpp"

int main(int argc, char **argv)
{
    int                 sockfd;
    struct sockaddr_in  servaddr;
    unsigned short      port = SERV_PORT;
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    if (argc != 2 && argc != 3)
        err_quit("usage: a.out <IPaddress> [port]");

    if (argc == 3)
        port = atoi(argv[2]);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_quit("socket error");

    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0)
        err_quit("setsockopt error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        err_quit("connect error");

    char snd_buf[1000] = {0};
    int i = 0;
    while (true) {
        if(send_all(sockfd, snd_buf, 1000) < 0) {
            err_quit("send_all error");
        }
        printf("send %d KB ok!\n", i);
        i++;
    }

    close(sockfd);

    exit(0);
}
