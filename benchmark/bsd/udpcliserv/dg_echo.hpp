#ifndef UNP_DG_ECHO_INC
#define UNP_DG_ECHO_INC

#include <sys/types.h>
#include <sys/socket.h>

void dg_echo(int, struct sockaddr *, socklen_t);

#endif
