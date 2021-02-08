#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "config.hpp"
#include "err_quit.hpp"
#include "utility.hpp"

int
main(int argc, char **argv)
{
	char			*ptr, **pptr;
	char			str[INET6_ADDRSTRLEN];
	struct hostent	*hptr;
	struct hostent	*rhptr;
    WSADATA         wsadata;

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

	while (--argc > 0) {
		ptr = *++argv;
		if ( (hptr = gethostbyname(ptr)) == NULL) {
			printf("gethostbyname error for host: %s: %s\n",
					ptr, WSAGetLastErrorString().c_str());
			continue;
		}
		printf("official hostname: %s\n", hptr->h_name);

		for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
			printf("	alias: %s\n", *pptr);

		switch (hptr->h_addrtype) {
		case AF_INET:
#ifdef	AF_INET6
		case AF_INET6:
#endif
			pptr = hptr->h_addr_list;
			for ( ; *pptr != NULL; pptr++) {
				printf("\taddress: %s\n",
					inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));

				if ( (rhptr = gethostbyaddr(*pptr, hptr->h_length,
										   hptr->h_addrtype)) == NULL)
					printf("\t(gethostbyaddr failed)\n");
				else if (rhptr->h_name != NULL)
					printf("\tname = %s\n", hptr->h_name);
				else
					printf("\t(no hostname returned by gethostbyaddr)\n");
			}
			break;

		default:
			printf("unknown address type\n");
			break;
		}
	}

    WSACleanup();

	exit(0);
}
