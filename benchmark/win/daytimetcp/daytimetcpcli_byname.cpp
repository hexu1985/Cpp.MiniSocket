#include <stdlib.h>
#include <stdio.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "tcp_connect.hpp"
#include "sock_ntop_host.hpp"

int
main(int argc, char **argv)
{
	int				sockfd, n;
	char			recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr_storage	ss;
    WSADATA             wsadata;

	if (argc != 3)
		err_quit("usage: daytimetcpcli <hostname/IPaddress> <service/port#>");

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

	sockfd = tcp_connect(argv[1], argv[2]);
	if (sockfd < 0)
		err_quit("tcp_connect error");

	len = sizeof(ss);
	if (getpeername(sockfd, (struct sockaddr *)&ss, &len) < 0)
		err_quit("getpeername error");
	printf("connected to %s\n", sock_ntop_host((struct sockaddr *)&ss, len));

	while ( (n = recv(sockfd, recvline, MAXLINE, 0)) > 0) {
		recvline[n] = 0;	/* null terminate */
		fputs(recvline, stdout);
	}
	if (n < 0) {
		err_quit("recv error");
	}
    WSACleanup();
	exit(0);
}
