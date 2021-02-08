#ifndef	UNP_SOCK_NTOP_HOST_INC
#define	UNP_SOCK_NTOP_HOST_INC

#include <winsock2.h>
#include <ws2tcpip.h>

char *sock_ntop_host(const struct sockaddr *, socklen_t);

#endif

