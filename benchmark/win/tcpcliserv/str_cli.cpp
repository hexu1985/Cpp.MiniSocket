#include "str_cli.hpp"

#include <stdio.h>
#include <string.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "send_all.hpp"
#include "recv_until.hpp"

void str_cli(FILE *fp, SOCKET sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL) {

        if (send_all(sockfd, sendline, strlen(sendline)) < 0)
            err_quit("send_all error");

        int ret = recv_until(sockfd, recvline, MAXLINE, '\n');
        if (ret < 0)
            err_quit("recv_until error");
        if (ret == 0)
            err_quit("str_cli: server terminated prematurely");

        fputs(recvline, stdout);
        fflush(stdout);
    }
}
