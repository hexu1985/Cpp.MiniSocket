#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <thread>

#include "config.hpp"
#include "err_quit.hpp"

int main(int argc, char **argv)
{
    int                listenfd, connfd;
    socklen_t          clilen;
    struct sockaddr_in cliaddr, servaddr;
    unsigned short     port = SERV_PORT;

    if (argc != 1 && argc != 2)
        err_quit("usage: a.out [listen_port]");

    if (argc == 2)
        port = atoi(argv[1]);

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_quit("socket error");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);  /* daytime server */

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        err_quit("bind error");

    if (listen(listenfd, 1) < 0)
        err_quit("listen error");

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

