#ifndef	UNP_SOCK_NTOP_INC
#define	UNP_SOCK_NTOP_INC

#include <winsock2.h>
#include <ws2tcpip.h>

char *sock_ntop(const struct sockaddr *, socklen_t);

#endif

