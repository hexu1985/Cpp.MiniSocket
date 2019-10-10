#ifndef UNP_UDP_SERVER_INC
#define UNP_UDP_SERVER_INC

#include <sys/types.h>
#include <sys/socket.h>

int udp_server(const char *, const char *, socklen_t *);

#endif
