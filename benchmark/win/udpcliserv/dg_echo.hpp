#ifndef UNP_DG_ECHO_INC
#define UNP_DG_ECHO_INC

#include <winsock2.h>
#include <ws2tcpip.h>

void dg_echo(SOCKET, struct sockaddr *, socklen_t);

#endif
