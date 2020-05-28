#ifndef UNP_CONNECT_TIMEO_INC
#define UNP_CONNECT_TIMEO_INC

#include <winsock2.h>
#include <ws2tcpip.h>

int      connect_timeo(SOCKET, const struct sockaddr *, socklen_t, int);

#endif
