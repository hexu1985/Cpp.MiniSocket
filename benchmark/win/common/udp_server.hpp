#ifndef UNP_UDP_SERVER_INC
#define UNP_UDP_SERVER_INC

#include <winsock2.h>
#include <ws2tcpip.h>

SOCKET udp_server(const char *, const char *, socklen_t *);

#endif
