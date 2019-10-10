#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "myaddrs.hpp"

char	**my_addrs(int *);

int
main(int argc, char **argv)
{
	int		addrtype;
	char	**pptr, buf[INET6_ADDRSTRLEN];

	if ( (pptr = my_addrs(&addrtype)) == NULL)
		err_quit("my_addrs error");

	for ( ; *pptr != NULL; pptr++)
		printf("\taddress: %s\n",
			   inet_ntop(addrtype, *pptr, buf, sizeof(buf)));

	exit(0);
}
