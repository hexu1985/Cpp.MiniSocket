/* include tcp_connect_timeo */
#include "tcp_connect_timeo.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "err_quit.hpp"
#include "connect_timeo.hpp"

int tcp_connect_timeo(const char *host, const char *serv, int nsec)
{
    int             sockfd, n;
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
        if (sockfd < 0)
            continue;  /* ignore this one */

        int ret = connect_timeo(sockfd, res->ai_addr, res->ai_addrlen, nsec);
        if (ret > 0) {
            break; /* success */
        } else if (ret == 0) {
            printf("connect timeout\n");
            continue;
        } else {
            printf("connect fail\n");
            continue;
        }

        close(sockfd);  /* ignore this one */
    } while ((res = res->ai_next) != NULL);

    if (res == NULL)  /* errno set from final connect() */
        err_quit("tcp_connect error for %s, %s", host, serv);

    freeaddrinfo(ressave);

    return (sockfd);
}
/* end tcp_connect */
