/* include tcp_connect */
#include "tcp_connect.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "err_quit.hpp"

SOCKET tcp_connect(const char *host, const char *serv)
{
    SOCKET          sockfd;
    int             n;
    struct addrinfo hints, *res, *ressave;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("tcp_connect error for %s, %s: %s",
                host, serv, gai_strerror(n));
    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd == INVALID_SOCKET)
            continue; /* ignore this one */

        if (connect(sockfd, res->ai_addr, res->ai_addrlen) != SOCKET_ERROR)
            break;  /* success */

        closesocket(sockfd); /* ignore this one */
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) /* errno set from final connect() */
        err_quit("tcp_connect error for %s, %s", host, serv);

    freeaddrinfo(ressave);

    return (sockfd);
}
/* end tcp_connect */
