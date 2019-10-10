#ifndef UNP_TCP_LISTEN_INC
#define UNP_TCP_LISTEN_INC

#include <sys/types.h>
#include <sys/socket.h>

int tcp_listen(const char *, const char *, socklen_t *);

#endif
