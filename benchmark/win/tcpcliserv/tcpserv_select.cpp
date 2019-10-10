#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "send_all.hpp"

int
main(int argc, char **argv)
{
	int					i, maxi;
    SOCKET              maxfd, listenfd, connfd, sockfd;
	int					nready; 
    SOCKET              client[FD_SETSIZE];
	ssize_t				n;
	fd_set				rset, allset;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
	unsigned short		port = SERV_PORT;
    char                str[INET_ADDRSTRLEN];
    WSADATA             wsadata;

    if (argc != 1 && argc != 2)
        err_quit("usage: a.out [listen_port]");

    if (argc == 2)
        port = atoi(argv[1]);

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        err_quit("socket error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(port);

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR)
        err_quit("bind error");

    if (listen(listenfd, LISTENQ) == SOCKET_ERROR)
        err_quit("listen error");

	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = INVALID_SOCKET;			/* INVALID_SOCKET indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
/* end fig01 */

/* include fig02 */
	for ( ; ; ) {
		rset = allset;		/* structure assignment */
		if ((nready = select(maxfd+1, &rset, NULL, NULL, NULL)) == SOCKET_ERROR)
			err_quit("select error");

		if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
			clilen = sizeof(cliaddr);
			if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) == SOCKET_ERROR)
				err_quit("accept error");

			printf("new client: %s, port %d\n",
					inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
					ntohs(cliaddr.sin_port));

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] == INVALID_SOCKET) {
					client[i] = connfd;	/* save descriptor */
					break;
				}
			if (i == FD_SETSIZE)
				err_quit("too many clients");

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i]) == INVALID_SOCKET)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				if ((n = recv(sockfd, buf, MAXLINE, 0)) == SOCKET_ERROR) {
					if (WSAGetLastError() == WSAECONNRESET) {
							/*4connection reset by client */
						printf("client[%d] aborted connection\n", i);
                        fflush(stdout);

						closesocket(sockfd);
						FD_CLR(sockfd, &allset);
						client[i] = INVALID_SOCKET;
					} else
						err_quit("recv error");
				} else if ( n == 0) {
						/*4connection closed by client */
					closesocket(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = INVALID_SOCKET;
				} else {
					send_all(sockfd, buf, n);
				}

				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}

    WSACleanup();
}
