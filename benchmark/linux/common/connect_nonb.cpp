#include "connect_nonb.hpp"
#include "err_quit.hpp"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

int connect_nonb(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
{
    int				flags, n, error;
    socklen_t		len;
    fd_set			rset, wset;
    struct timeval	tval;

    // 获取套接字flags
    if ((flags = fcntl(sockfd, F_GETFL, 0)) < 0) {
        return -1;
    }

    // 设置套接字为非阻塞的
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        return -1;
    }

    error = 0;
    if ( (n = connect(sockfd, saptr, salen)) < 0) {
        if (errno != EINPROGRESS) {
            error = errno;
            goto done;
        }
    }

    /* Do whatever we want while the connect is taking place. */

    if (n == 0)
        goto done;	/* connect completed immediately */

    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;
    tval.tv_sec = nsec;
    tval.tv_usec = 0;

    n = select(sockfd+1, &rset, &wset, NULL, nsec ? &tval : NULL);
    if (n == 0) {
        close(sockfd);		/* timeout */
        error = ETIMEDOUT;
    } else if (n < 0) {
        error = errno;
    } else {    // n == 0
        if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
            len = sizeof(error);
            if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
                error = errno;
            }
        } else {
            error = -1;
        }
    }

done:
    fcntl(sockfd, F_SETFL, flags);	/* restore file status flags */

    if (error) {
        close(sockfd);		/* just in case */
        errno = error;
        return(-1);
    }
    return(0);
}
