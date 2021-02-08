#ifndef UNP_CONFIG_INC
#define UNP_CONFIG_INC

#include <limits.h>

#ifndef OPEN_MAX
#define OPEN_MAX    1024
#endif

/* POSIX requires that an #include of <poll.h> DefinE INFTIM, but many                                                                                                 
 * systems still DefinE it in <sys/stropts.h>.  We don't want to include
 * all the STREAMS stuff if it's not needed, so we just DefinE INFTIM here.
 * This is the standard value, but there's no guarantee it is -1. */
#ifndef INFTIM
#define INFTIM          (-1)    /* infinite poll timeout */
#endif

#ifndef MAXLINE
#define	MAXLINE	4096
#endif

#ifndef LISTENQ 
#define LISTENQ 1024
#endif

#ifndef SERV_PORT 
#define SERV_PORT 9877
#endif

#endif
