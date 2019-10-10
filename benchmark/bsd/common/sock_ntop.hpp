#ifndef	__unp_sock_ntop_h
#define	__unp_sock_ntop_h

#include <netinet/in.h>
#include <arpa/inet.h>

char *sock_ntop(const struct sockaddr *, socklen_t);

#endif

