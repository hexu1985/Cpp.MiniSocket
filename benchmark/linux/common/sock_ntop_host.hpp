#ifndef	UNP_SOCK_NTOP_HOST_INC
#define	UNP_SOCK_NTOP_HOST_INC

#include <netinet/in.h>
#include <arpa/inet.h>

char *sock_ntop_host(const struct sockaddr *, socklen_t);

#endif

