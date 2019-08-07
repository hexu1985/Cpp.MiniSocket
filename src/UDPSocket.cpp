#include "UDPSocket.hpp"
#include "SYSException.hpp"

namespace MiniSocket {

UDPSocket::UDPSocket(const SocketAddress &localAddress)
{
    int domain = localAddress.getSockaddr()->sa_family;
    createSocket(domain, SOCK_DGRAM, 0);
    bind(localAddress);
}

int UDPSocket::sendTo(const char *buffer, int bufferLen,
        const SocketAddress &foreignAddress)
{
    if (!isOpened()) {
        int domain = foreignAddress.getSockaddr()->sa_family;
        createSocket(domain, SOCK_DGRAM, 0);
    }

    int n = ::sendto(sockDesc_, buffer, bufferLen, 0,
            foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen());
    if ( n < 0 ) {
        sys_error("Send failed (sendto())");
    }

    return n;
}

int UDPSocket::recvFrom(char *buffer, int bufferLen,
            SocketAddress &sourceAddress)
{
    sockaddr_storage cliAddr;
    socklen_t addrLen = sizeof(cliAddr);
    int n = recvfrom(sockDesc_, buffer, bufferLen, 0,
            (sockaddr *) &cliAddr, (socklen_t *) &addrLen);
    if (n < 0) {
        sys_error("Receive failed (recvfrom())");
    }
    sourceAddress = SocketAddress((sockaddr *)&cliAddr, addrLen);

    return n;
}

}   // namesapce MiniSocket
