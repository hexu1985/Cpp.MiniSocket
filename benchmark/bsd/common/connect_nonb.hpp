#ifndef UNP_CONNECT_TIMEO_INC
#define UNP_CONNECT_TIMEO_INC

#include <sys/types.h>
#include <sys/socket.h>

int connect_nonb(int, const struct sockaddr *, socklen_t, int);

#endif
