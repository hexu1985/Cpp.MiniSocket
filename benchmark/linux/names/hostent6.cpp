#define _GNU_SOURCE
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i, ret;
    struct gaicb *reqs[argc - 1];
    char host[NI_MAXHOST];
    struct addrinfo *res;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s HOST...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < argc - 1; i++) {
        reqs[i] = (struct gaicb *) malloc(sizeof(*reqs[0]));
        if (reqs[i] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        memset(reqs[i], 0, sizeof(*reqs[0]));
        reqs[i]->ar_name = argv[i + 1];
    }

    ret = getaddrinfo_a(GAI_NOWAIT, reqs, argc - 1, NULL);
    if (ret != 0) {
        fprintf(stderr, "getaddrinfo_a() failed: %s\n",
                gai_strerror(ret));
        exit(EXIT_FAILURE);
    }

    ret = gai_suspend(reqs, argc - 1, NULL);
    if (ret) {
        fprintf(stderr, "gai_suspend(): %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < argc - 1; i++) {
        printf("%s: ", reqs[i]->ar_name);
        ret = gai_error(reqs[i]);
        if (ret == 0) {
            res = reqs[i]->ar_result;

            ret = getnameinfo(res->ai_addr, res->ai_addrlen,
                    host, sizeof(host),
                    NULL, 0, NI_NUMERICHOST);
            if (ret != 0) {
                fprintf(stderr, "getnameinfo() failed: %s\n",
                        gai_strerror(ret));
                exit(EXIT_FAILURE);
            }
            puts(host);

        } else {
            puts(gai_strerror(ret));
        }
    }
    exit(EXIT_SUCCESS);
}
