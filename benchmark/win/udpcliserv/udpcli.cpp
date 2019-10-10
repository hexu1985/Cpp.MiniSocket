#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "dg_cli.hpp"

int
main(int argc, char **argv)
{
	SOCKET				sockfd;
	struct sockaddr_in	servaddr;
	unsigned short      port = SERV_PORT;
    WSADATA             wsadata;

	if (argc != 2 && argc != 3)
		err_quit("usage: udpcli <IPaddress> [port]");

	if (argc == 3)
		port = atoi(argv[2]);

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		err_quit("socket error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	dg_cli(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	exit(0);
}
