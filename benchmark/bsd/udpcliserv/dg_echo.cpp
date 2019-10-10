#include "dg_echo.hpp"
#include "config.hpp"
#include "err_quit.hpp"

void
dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
	int			n;
	socklen_t	len;
	char		mesg[MAXLINE];

	for ( ; ; ) {
		len = clilen;
		n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
		if (n < 0)
			err_quit("recvfrom error");

		if (sendto(sockfd, mesg, n, 0, pcliaddr, len) != n)
			err_quit("sendto error");
	}
}
