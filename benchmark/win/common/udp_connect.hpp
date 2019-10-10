#ifndef UNP_UDP_CONNECT_INC
#define UNP_UDP_CONNECT_INC

#include <winsock2.h>
#include <ws2tcpip.h>

SOCKET udp_connect(const char *, const char *);

#endif
