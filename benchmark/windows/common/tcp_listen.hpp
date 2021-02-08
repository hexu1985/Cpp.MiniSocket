#ifndef UNP_TCP_LISTEN_INC
#define UNP_TCP_LISTEN_INC

#include <winsock2.h>
#include <ws2tcpip.h>

SOCKET tcp_listen(const char *, const char *, socklen_t *);

#endif
