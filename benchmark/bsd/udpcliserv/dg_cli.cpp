#include <string.h>

#include "dg_cli.hpp"
#include "config.hpp"
#include "err_quit.hpp"

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int  n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    while (fgets(sendline, MAXLINE, fp) != NULL) {

        n = strlen(sendline);
        if (sendto(sockfd, sendline, n, 0, pservaddr, servlen) != n)
            err_quit("sendto error");

        n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
        if (n < 0)
            err_quit("recvfrom error");

        recvline[n] = 0;  /* null terminate */
        fputs(recvline, stdout);
    }
}
