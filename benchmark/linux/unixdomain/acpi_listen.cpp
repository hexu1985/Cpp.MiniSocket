#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "recv_until.hpp"

void
acpi_cli(int sockfd);

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_un	servaddr;

	if ((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
        err_quit("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, "/var/run/acpid.socket");

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        err_quit("connect error");

	acpi_cli(sockfd);		/* do it all */

	exit(0);
}

#if 0
void
acpi_cli(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];

again:
	while ( (n = recv(sockfd, buf, MAXLINE-1, 0)) > 0) {
        buf[n] = '\0';
        fprintf(stderr, "%s", buf);
    }

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_quit("str_echo: read error");
}
#else
void
acpi_cli(int sockfd)
{
	char	recvline[MAXLINE];

    for ( ; ; ) {

		if (recv_until(sockfd, recvline, MAXLINE, '\n') <= 0)
			err_quit("str_cli: server terminated prematurely");

		fputs(recvline, stdout);
	}
}
#endif
