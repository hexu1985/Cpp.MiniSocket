#include "send_all.hpp"

#include <sys/socket.h>

#include <errno.h>

ssize_t						/* Write "buf_len" bytes to a descriptor. */
send_all(int sockfd, const void *buffer, size_t buf_len)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = (const char *) buffer;
	nleft = buf_len;
	while (nleft > 0) {
		if ( (nwritten = send(sockfd, ptr, nleft, 0)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(buf_len);
}
