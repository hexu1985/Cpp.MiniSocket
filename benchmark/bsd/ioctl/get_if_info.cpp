#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "err_quit.hpp"
#include "sock_ntop.hpp"

void get_if_info(int sockfd);
void get_if_hwaddr(int sockfd, const char *ifname);
void get_if_ipaddr(int sockfd, const char *ifname);
void get_if_netmask(int sockfd, const char *ifname);
void get_if_flags(int sockfd, const char *ifname);
void get_if_mtu(int sockfd, const char *ifname);

int main(int argc, char *argv[])
{
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        err_quit("socket error");

    get_if_info(sockfd);

    return 0;
}

void get_if_info(int sockfd)
{
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
    printf("\n");

    for (int i = 0; i < if_count; i++) {
        printf("%s", ifr_arr[i].ifr_name);

        printf("\n\t\t");
        get_if_hwaddr(sockfd, ifr_arr[i].ifr_name);

        printf("\n\t\t");
        get_if_ipaddr(sockfd, ifr_arr[i].ifr_name);
        printf("\t");
        get_if_netmask(sockfd, ifr_arr[i].ifr_name);

        printf("\n\t\t");
        get_if_flags(sockfd, ifr_arr[i].ifr_name);
        printf("\t");
        get_if_mtu(sockfd, ifr_arr[i].ifr_name);

        printf("\n\n");

    }

}

void get_if_hwaddr(int sockfd, const char *ifname)
{
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) 
        err_quit("ioctl error: SIOCGIFHWADDR");

    const uint8_t *mac = NULL;
    printf("Link encap:");
    switch(ifr.ifr_hwaddr.sa_family) {
        case ARPHRD_LOOPBACK:
            printf("LOOPBACK");
            break;
        case ARPHRD_NETROM:
            printf("NETROM");
            break;
        case ARPHRD_ETHER:
            mac = (const uint8_t *) ifr.ifr_hwaddr.sa_data;   // ETH_ALEN
            printf("ETHER");
            printf("\t");
            printf("MAC address: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
                        *mac, *(mac+1), *(mac+2), *(mac+3), *(mac+4), *(mac+5));
            break;
        case ARPHRD_PPP:
            printf("PPP");
            break;
        case ARPHRD_EETHER:
            printf("EETHER");
            break;
        case ARPHRD_IEEE802:
            printf("IEEE802");
            break;
        default:
            printf("UNKNOWN(%d)", ifr.ifr_hwaddr.sa_family);
            break;
    }
}

void get_if_ipaddr(int sockfd, const char *ifname)
{
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

    if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0) {
        err_quit("ioctl error: SIOCGIFADDR");
    }

    printf("inet addr:%s", sock_ntop(&ifr.ifr_addr, 0));
}

void get_if_netmask(int sockfd, const char *ifname)
{
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

    if (ioctl(sockfd, SIOCGIFNETMASK, &ifr) < 0) {
        err_quit("ioctl error: SIOCGIFNETMASK");
    }

    printf("netmask:%s", sock_ntop(&ifr.ifr_addr, 0));
}

void get_if_flags(int sockfd, const char *ifname)
{
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
        err_quit("ioctl error: SIOCGIFFLAGS");
    }

    short flags = ifr.ifr_flags;

    if (flags & IFF_UP)
        printf("UP ");

    if (flags & IFF_RUNNING)
        printf("RUNNING ");

    if (flags & IFF_LOOPBACK)
        printf("LOOPBACK ");

    if (flags & IFF_BROADCAST)
        printf("BROADCAST ");

    if (flags & IFF_MULTICAST)
        printf("MULTICAST ");

    if (flags & IFF_PROMISC)
        printf("PROMISC");
}

void get_if_mtu(int sockfd, const char *ifname)
{
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

    if (ioctl(sockfd, SIOCGIFMTU, &ifr) < 0) {
        err_quit("ioctl error: SIOCGIFMTU");
    }

    printf("MTU:%d", ifr.ifr_mtu);
}
