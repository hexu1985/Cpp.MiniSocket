#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "send_all.hpp"

int main(int argc, char **argv)
{
    int                i, maxi, maxfd, listenfd, connfd, sockfd;
    int                nready, client[FD_SETSIZE];
    ssize_t            n;
    fd_set             rset, allset;
    char               buf[MAXLINE];
    socklen_t          clilen;
    struct sockaddr_in cliaddr, servaddr;
    unsigned short     port = SERV_PORT;
    char               str[INET_ADDRSTRLEN];

    if (argc != 1 && argc != 2)
        err_quit("usage: a.out [listen_port]");

    if (argc == 2)
        port = atoi(argv[1]);

    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_quit("socket error");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        err_quit("bind error");

    if (listen(listenfd, LISTENQ) < 0)
        err_quit("listen error");

    maxfd = listenfd;      /* initialize */
    maxi = -1;          /* index into client[] array */
    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;      /* -1 indicates available entry */
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    /* end fig01 */

    /* include fig02 */
    for ( ; ; ) {
        rset = allset;    /* structure assignment */
        if ((nready = select(maxfd+1, &rset, NULL, NULL, NULL)) < 0)
            err_quit("select error");

        if (FD_ISSET(listenfd, &rset)) {  /* new client connection */
            clilen = sizeof(cliaddr);
            if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0)
                err_quit("accept error");

            printf("new client: %s, port %d\n",
                    inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
                    ntohs(cliaddr.sin_port));

            for (i = 0; i < FD_SETSIZE; i++)
                if (client[i] < 0) {
                    client[i] = connfd;  /* save descriptor */
                    break;
                }
            if (i == FD_SETSIZE)
                err_quit("too many clients");

            FD_SET(connfd, &allset);  /* add new descriptor to set */
            if (connfd > maxfd)
                maxfd = connfd;      /* for select */
            if (i > maxi)
                maxi = i;        /* max index in client[] array */

            if (--nready <= 0)
                continue;        /* no more readable descriptors */
        }

        for (i = 0; i <= maxi; i++) {  /* check all clients for data */
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                if ((n = recv(sockfd, buf, MAXLINE, 0)) < 0) {
                    if (errno == ECONNRESET) {
                        /*4connection reset by client */
                        printf("client[%d] aborted connection\n", i);
                        close(sockfd);
                        FD_CLR(sockfd, &allset);
                        client[i] = -1;
                    } else
                        err_quit("recv error");
                } else if ( n == 0) {
                    /*4connection closed by client */
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    send_all(sockfd, buf, n);
                }

                if (--nready <= 0)
                    break;        /* no more readable descriptors */
            }
        }
    }
}
