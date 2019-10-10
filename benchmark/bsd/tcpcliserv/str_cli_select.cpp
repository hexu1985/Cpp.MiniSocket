#include "str_cli.hpp"

#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "config.hpp"
#include "err_quit.hpp"
#include "send_all.hpp"
#include "recv_until.hpp"

void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];

	FD_ZERO(&rset);
	for ( ; ; ) {
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = std::max(fileno(fp), sockfd) + 1;
		if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0)
			err_quit("select error");

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			int ret = recv_until(sockfd, recvline, MAXLINE, '\n');
			if (ret < 0)
				err_quit("recv_until error");
			if (ret == 0)
				err_quit("str_cli: server terminated prematurely");
			fputs(recvline, stdout);
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if (fgets(sendline, MAXLINE, fp) == NULL)
				return;		/* all done */
			if (send_all(sockfd, sendline, strlen(sendline)) < 0)
				err_quit("send_all error");
		}
	}
}
