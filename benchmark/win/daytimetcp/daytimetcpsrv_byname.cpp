#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "sock_ntop.hpp"
#include "tcp_listen.hpp"

int
main(int argc, char **argv)
{
	int				listenfd, connfd;
	socklen_t		len, addrlen;
	char			buff[MAXLINE];
	time_t			ticks;
	struct sockaddr_storage	cliaddr;
    WSADATA             wsadata;

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

	if (argc == 2)
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: daytimetcpsrv2 [ <host> ] <service or port>");

	if (listenfd < 0)
		err_quit("tcp_listen error");

	for ( ; ; ) {
		len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
		if (connfd < 0)
			err_quit("accept error");
		printf("connection from %s\n", sock_ntop((struct sockaddr *)&cliaddr, len));

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		if (send(connfd, buff, strlen(buff), 0) != strlen(buff))
			err_quit("send error");

		closesocket(connfd);
	}

    WSACleanup();
}
