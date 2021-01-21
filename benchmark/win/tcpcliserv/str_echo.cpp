#include "str_echo.hpp"

#include <stdio.h>

#include "config.hpp"
#include "send_all.hpp"

void str_echo(SOCKET sockfd)
{
    int  n;
    char buf[MAXLINE];

    while ((n = recv(sockfd, buf, MAXLINE, 0)) > 0)
        send_all(sockfd, buf, n);

    if (n == SOCKET_ERROR)
        printf("str_echo: read error\n");
}
