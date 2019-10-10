#include <stdlib.h>

#include "err_quit.hpp"
#include "str_cli.hpp"
#include "tcp_connect.hpp"
#include "config.hpp"

int
main(int argc, char **argv)
{
	SOCKET				sockfd;
    WSADATA             wsadata;

	if (argc != 3)
		err_quit("usage: a.out <hostname/IPaddress> <service/port#>");

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

	sockfd = tcp_connect(argv[1], argv[2]);
	if (sockfd < 0)
		err_quit("tcp_connect error");

	str_cli(stdin, sockfd);		/* do it all */

    WSACleanup();
	exit(0);
}
