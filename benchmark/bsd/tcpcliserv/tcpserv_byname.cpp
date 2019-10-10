#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <thread>

#include "config.hpp"
#include "err_quit.hpp"
#include "str_echo.hpp"
#include "tcp_listen.hpp"

static void	doit(int sockfd);		/* each thread executes this function */

int
main(int argc, char **argv)
{
	int				listenfd, connfd;
	socklen_t		addrlen, len;
	struct sockaddr	*cliaddr;

	if (argc == 2)
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: a.out [ <host> ] <service or port>");

	if (listenfd < 0)
		err_quit("tcp_listen error");

	cliaddr = (struct sockaddr *) malloc(addrlen);
	if (cliaddr == NULL)
		err_quit("malloc error");

	for ( ; ; ) {
		len = addrlen;
		if ( (connfd = accept(listenfd, cliaddr, &len)) < 0)
			err_quit("accept error");

		std::thread(doit, connfd).detach();
	}
}

static void
doit(int sockfd)
{
	str_echo(sockfd);	/* process the request */
	close(sockfd);		/* done with connected socket */
}
