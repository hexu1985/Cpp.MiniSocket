#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "config.hpp"
#include "err_quit.hpp"
#include "sock_ntop.hpp"

int
main(int argc, char **argv)
{
	int					sockfd;
	ssize_t				n;
	char				buff[MAXLINE];
	time_t				ticks;
	socklen_t			len;
	struct sockaddr_in	servaddr, cliaddr;
	unsigned short		port = SERV_PORT;

    if (argc != 1 && argc != 2)
        err_quit("usage: a.out [listen_port]");

    if (argc == 2)
        port = atoi(argv[1]);

	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        err_quit("usage: a.out [listen_port]");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(port);

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        err_quit("bind error");

	for ( ; ; ) {
		len = sizeof(cliaddr);
		n = recvfrom(sockfd, buff, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
		if (n < 0)
			err_quit("recvfrom error");
		printf("datagram from %s\n", sock_ntop((struct sockaddr *)&cliaddr, len));

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		if (sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&cliaddr, len) != strlen(buff))
			err_quit("sendto error");
	}
}
