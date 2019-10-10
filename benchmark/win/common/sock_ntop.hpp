#ifndef	__unp_sock_ntop_h
#define	__unp_sock_ntop_h

#include <winsock2.h>
#include <ws2tcpip.h>

char *sock_ntop(const struct sockaddr *, socklen_t);

#endif

