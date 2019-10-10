#include <winsock2.h>
#include <ws2tcpip.h>

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
    WSADATA         wsadata;

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

	if ( (pptr = my_addrs(&addrtype)) == NULL)
		err_quit("my_addrs error");

	for ( ; *pptr != NULL; pptr++)
		printf("\taddress: %s\n",
			   inet_ntop(addrtype, *pptr, buf, sizeof(buf)));

    WSACleanup();

	exit(0);
}
