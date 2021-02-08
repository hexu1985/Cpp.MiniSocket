#ifndef UNP_TCP_CONNECT_INC
#define UNP_TCP_CONNECT_INC

#include <winsock2.h>
#include <ws2tcpip.h>

SOCKET tcp_connect(const char *, const char *);

#endif
