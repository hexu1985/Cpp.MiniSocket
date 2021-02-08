#ifndef UNP_DG_CLI_INC
#define UNP_DG_CLI_INC

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

void dg_cli(FILE *, int, const struct sockaddr *, socklen_t);

#endif
