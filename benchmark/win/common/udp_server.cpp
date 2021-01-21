/* include udp_server */
#include "udp_server.hpp"

#include <string.h>

#include "err_quit.hpp"
#include "config.hpp"

SOCKET udp_server(const char *host, const char *serv, socklen_t *addrlenp)
{
    SOCKET          sockfd;
    int             n;
    struct addrinfo hints, *res, *ressave;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("udp_server error for %s, %s: %s",
                host, serv, gai_strerror(n));
    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd == INVALID_SOCKET)
            continue;  /* error - try next one */

        if (bind(sockfd, res->ai_addr, res->ai_addrlen) == 0)
            break;   /* success */

        closesocket(sockfd);  /* bind error - close and try next one */
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) /* errno from final socket() or bind() */
        err_quit("udp_server error for %s, %s", host, serv);

    if (addrlenp)
        *addrlenp = res->ai_addrlen; /* return size of protocol address */

    freeaddrinfo(ressave);

    return(sockfd);
}
/* end udp_server */

