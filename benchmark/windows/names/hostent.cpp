#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "config.hpp"
#include "err_quit.hpp"
#include "utility.hpp"

int main(int argc, char **argv)
{
    char           *ptr, **pptr;
    char            str[INET_ADDRSTRLEN];
    struct hostent *hptr;
    WSADATA         wsadata;

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    while (--argc > 0) {
        ptr = *++argv;
        if ((hptr = gethostbyname(ptr)) == NULL) {
            printf("gethostbyname error for host: %s: %s\n",
                    ptr, WSAGetLastErrorString().c_str());
            continue;
        }
        printf("official hostname: %s\n", hptr->h_name);

        for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
            printf("\talias: %s\n", *pptr);

        switch (hptr->h_addrtype) {
        case AF_INET:
            pptr = hptr->h_addr_list;
            for ( ; *pptr != NULL; pptr++)
                printf("\taddress: %s\n",
                        inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
            break;

        default:
            printf("unknown address type\n");
            break;
        }
    }

    WSACleanup();

    exit(0);
}
