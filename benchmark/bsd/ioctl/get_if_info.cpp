#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "err_quit.hpp"

int main(int argc, char *argv[])
{
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        err_quit("socket error");

    struct ifconf ifc;
    struct ifreq ifr_arr[16];

    memset(&ifc, 0, sizeof(struct ifconf));
    ifc.ifc_len = 10 * sizeof(struct ifreq);
    ifc.ifc_buf = (char *) ifr_arr;

    /* SIOCGIFCONF is IP specific. see netdevice(7) */
    if (ioctl(sockfd, SIOCGIFCONF, (char *) &ifc) < 0)
        err_quit("ioctl error: SIOCGIFCONF");

    int if_count = ifc.ifc_len / (sizeof(struct ifreq));
    printf("interface num = %d\n", if_count);

    for (int i = 0; i < if_count; i++) {
    }


    return 0;
}
