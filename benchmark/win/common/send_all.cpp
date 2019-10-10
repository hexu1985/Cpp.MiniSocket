#include "send_all.hpp"

int						/* Write "buf_len" bytes to a descriptor. */
send_all(SOCKET sockfd, const void *buffer, int buf_len)
{
	int		     nleft;
	int          nwritten;
	const char	*ptr;

	ptr = (const char *) buffer;
	nleft = buf_len;
	while (nleft > 0) {
        nwritten = send(sockfd, ptr, nleft, 0);
		if ( nwritten == 0 || nwritten == SOCKET_ERROR ) {
            return(-1);			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(buf_len);
}
