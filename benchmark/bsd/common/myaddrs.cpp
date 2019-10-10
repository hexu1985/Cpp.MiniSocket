#include "myaddrs.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <stddef.h>

char **
my_addrs(int *addrtype)
{
	struct hostent	*hptr;
	char			myname[4096];

	if (gethostname(myname, sizeof(myname)) < 0)
		return(NULL);

	if ( (hptr = gethostbyname(myname)) == NULL)
		return(NULL);

	*addrtype = hptr->h_addrtype;
	return(hptr->h_addr_list);
}
