#include "recv_all.hpp"

#include <sys/socket.h>

#include <errno.h>

ssize_t recv_all(int fd, void *vptr, size_t n) /* Read "n" bytes from a descriptor. */
{
    size_t  nleft;
    ssize_t nread;
    char   *ptr;

    ptr = (char *) vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = recv(fd, ptr, nleft, 0)) < 0) {
            if (errno == EINTR)
                nread = 0;    /* and call read() again */
            else
                return (-1);
        } else if (nread == 0)
            break;        /* EOF */

        nleft -= nread;
        ptr   += nread;
    }
    return (n - nleft);    /* return >= 0 */
}
