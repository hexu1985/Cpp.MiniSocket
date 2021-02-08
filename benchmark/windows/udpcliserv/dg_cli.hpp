#ifndef UNP_DG_CLI_INC
#define UNP_DG_CLI_INC

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

void dg_cli(FILE *, SOCKET, const struct sockaddr *, socklen_t);

#endif
