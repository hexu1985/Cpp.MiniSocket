#ifndef	UNP_SOCK_NTOP_INC
#define	UNP_SOCK_NTOP_INC

#include <netinet/in.h>
#include <arpa/inet.h>

char *sock_ntop(const struct sockaddr *, socklen_t);

#endif

