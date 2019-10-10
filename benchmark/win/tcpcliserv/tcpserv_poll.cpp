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
	ssize_t				n;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct pollfd		client[OPEN_MAX];
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

	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;
	for (i = 1; i < OPEN_MAX; i++)
		client[i].fd = INVALID_SOCKET;		/* INVALID_SOCKET indicates available entry */
	maxi = 0;					/* max index into client[] array */
/* end fig01 */

/* include fig02 */
	for ( ; ; ) {
		if ((nready = WSAPoll(client, maxi+1, INFTIM)) == SOCKET_ERROR)
			err_quit("WSAPoll error");

		if (client[0].revents & POLLRDNORM) {	/* new client connection */
			clilen = sizeof(cliaddr);
			if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) == SOCKET_ERROR)
				err_quit("accept error");

			printf("new client: %s, port %d\n",
					inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
					ntohs(cliaddr.sin_port));

			for (i = 1; i < OPEN_MAX; i++)
				if (client[i].fd == INVALID_SOCKET) {
					client[i].fd = connfd;	/* save descriptor */
					break;
				}
			if (i == OPEN_MAX)
				err_quit("too many clients");

			client[i].events = POLLRDNORM;
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		for (i = 1; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i].fd) == INVALID_SOCKET)
				continue;
			if (client[i].revents & (POLLRDNORM | POLLERR)) {
				if ( (n = recv(sockfd, buf, MAXLINE, 0)) < 0) {
					if (WSAGetLastError() == WSAECONNRESET) {
							/*4connection reset by client */
						printf("client[%d] aborted connection\n", i);
                        fflush(stdout);

						closesocket(sockfd);
						client[i].fd = INVALID_SOCKET;
					} else
						err_quit("recv error");
				} else if (n == 0) {
						/*4connection closed by client */
					printf("client[%d] closed connection\n", i);
                    fflush(stdout);

					closesocket(sockfd);
					client[i].fd = INVALID_SOCKET;
				} else {
					send_all(sockfd, buf, n);
				}

				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
}
