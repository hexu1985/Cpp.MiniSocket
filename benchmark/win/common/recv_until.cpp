#include "recv_until.hpp"

#include "config.hpp"

static int recv_cnt;
static char *recv_ptr;
static char recv_buf[MAXLINE];

static int my_recv(SOCKET sockfd, char *ptr)
{
    if (recv_cnt <= 0) {
        if ((recv_cnt = recv(sockfd, recv_buf, sizeof(recv_buf), 0)) == SOCKET_ERROR) {
            return (-1);
        } else if (recv_cnt == 0)
            return (0);
        recv_ptr = recv_buf;
    }

    recv_cnt--;
    *ptr = *recv_ptr++;
    return (1);
}

int recv_until(SOCKET sockfd, void *buffer, int maxlen, char term)
{
    int  n, rc;
    char c, *ptr;

    ptr = (char *) buffer;
    for (n = 1; n < maxlen; n++) {
        if ((rc = my_recv(sockfd, &c)) == 1) {
            *ptr++ = c;
            if (c == term)
                break; /* newline is stored, like fgets() */
        } else if (rc == 0) {
            *ptr = 0;
            return (n - 1); /* EOF, n - 1 bytes were recv */
        } else
            return (-1);  /* error, errno set by recv() */
    }

    *ptr = 0; /* null terminate like fgets() */
    return (n);
}

