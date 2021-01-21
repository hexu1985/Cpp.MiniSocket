#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "config.hpp"
#include "err_quit.hpp"
#include "utility.hpp"
#include "sock_ntop.hpp"

int main(int argc, char **argv)
{
    int             n;
    char           *ptr;
    struct addrinfo hints, *res, *ressave;
    WSADATA         wsadata;

    if (WSAStartup(WSVERS, &wsadata) != 0)
        err_quit("WSAStartup failed");

    while (--argc > 0) {
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_flags = AI_CANONNAME;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        ptr = *++argv;
        if ((n = getaddrinfo(ptr, NULL, &hints, &res)) != 0) {
            printf("getaddrinfo error for host %s: %s\n", 
                    ptr, WSAGetLastErrorString().c_str());
            printf("getaddrinfo error for host %s: %s\n", 
                    ptr, gai_strerror(n));
            continue;
        }
        printf("official host name: %s\n", res->ai_canonname);

        ressave = res;
        do {
            printf("\taddress: %s\n",
                    sock_ntop(res->ai_addr, res->ai_addrlen));
        } while ((res = res->ai_next) != NULL);

        freeaddrinfo(ressave);
    }

    WSACleanup();

    exit(0);
}

