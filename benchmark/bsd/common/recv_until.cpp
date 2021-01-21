#include "recv_until.hpp"

#include <sys/socket.h>

#include <errno.h>

#include "config.hpp"

static int  recv_cnt;
static char  *recv_ptr;
static char  recv_buf[MAXLINE];

static ssize_t my_recv(int sockfd, char *ptr)
{
    if (recv_cnt <= 0) {
again:
        if ((recv_cnt = recv(sockfd, recv_buf, sizeof(recv_buf), 0)) < 0) {
            if (errno == EINTR)
                goto again;
            return (-1);
        } else if (recv_cnt == 0)
            return (0);
        recv_ptr = recv_buf;
    }

    recv_cnt--;
    *ptr = *recv_ptr++;
    return (1);
}

ssize_t recv_until(int sockfd, void *buffer, size_t maxlen, char term)
{
    ssize_t  n, rc;
    char  c, *ptr;

    ptr = (char *) buffer;
    for (n = 1; n < maxlen; n++) {
        if ((rc = my_recv(sockfd, &c)) == 1) {
            *ptr++ = c;
            if (c == term)
                break;  /* newline is stored, like fgets() */
        } else if (rc == 0) {
            *ptr = 0;
            return (n - 1);  /* EOF, n - 1 bytes were recv */
        } else
            return (-1);    /* error, errno set by recv() */
    }

    *ptr = 0;  /* null terminate like fgets() */
    return (n);
}

ssize_t recv_until(int sockfd, void *buffer, size_t maxlen)
{
    ssize_t  n, rc;
    char  c, *ptr;

    ptr = (char *) buffer;
    for (n = 0; n < maxlen; n++) {
        if ((rc = my_recv(sockfd, &c)) == 1) {
            *ptr++ = c;
        } else if (rc == 0) {
            *ptr = 0;
            return (n - 1);  /* EOF, n - 1 bytes were recv */
        } else
            return (-1);    /* error, errno set by recv() */
    }

    return (n);
}
