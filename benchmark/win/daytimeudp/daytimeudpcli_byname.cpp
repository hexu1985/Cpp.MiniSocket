#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "udp_connect.hpp"

int
main(int argc, char **argv)
{
	int		sockfd, n;
	char	recvline[MAXLINE + 1];
    WSADATA             wsadata;

	if (argc != 3)
		err_quit("usage: a.out <hostname/IPaddress> <service/port#>");

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

	sockfd = udp_connect(argv[1], argv[2]);

	send(sockfd, "", 1, 0);	/* send 1-byte datagram */

	n = recv(sockfd, recvline, MAXLINE, 0);
	recvline[n] = '\0';	/* null terminate */
	fputs(recvline, stdout);

    WSACleanup();
	exit(0);
}
