#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "config.hpp"
#include "err_quit.hpp"
#include "utility.hpp"

void	pr_ipv4(char **);

int
main(int argc, char **argv)
{
	char			*ptr, **pptr;
	struct hostent	*hptr;
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
		printf("official host name: %s\n", hptr->h_name);

		for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
			printf("	alias: %s\n", *pptr);

		switch (hptr->h_addrtype) {
		case AF_INET:
			pr_ipv4(hptr->h_addr_list);
			break;

		default:
			printf("unknown address type\n");
			break;
		}
	}

    WSACleanup();

	exit(0);
}

/*
 * Print the array of IPv4 addresses that is returned.
 * Also call gethostbyaddr_r() for each IP address and print the name.
 */

/* begin pr_ipv4 */
void
pr_ipv4(char **listptr)
{
	struct in_addr	inaddr;
	struct hostent	*hptr;
	char			buf[8192];

	for ( ; *listptr != NULL; listptr++) {
		inaddr = *((struct in_addr *) (*listptr));
		printf("	IPv4 address: %s", inet_ntoa(inaddr));


        if ( (hptr = gethostbyaddr((char *) &inaddr, sizeof(struct in_addr), AF_INET)) == NULL)
			printf("    (gethostbyaddr failed: %s)\n", WSAGetLastErrorString().c_str());
		else if (hptr->h_name != NULL)
			printf("    name = %s\n", hptr->h_name);
		else
			printf("    (no hostname returned by gethostbyaddr)\n");
	}
}
/* end pr_ipv4 */
