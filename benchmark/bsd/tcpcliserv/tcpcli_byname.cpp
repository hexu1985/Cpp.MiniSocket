#include <stdlib.h>

#include "err_quit.hpp"
#include "str_cli.hpp"
#include "tcp_connect.hpp"

int
main(int argc, char **argv)
{
	int					sockfd;

	if (argc != 3)
		err_quit("usage: a.out <hostname/IPaddress> <service/port#>");

	sockfd = tcp_connect(argv[1], argv[2]);
	if (sockfd < 0)
		err_quit("tcp_connect error");

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
