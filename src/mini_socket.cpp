#include "mini_socket.hpp"
#include "SYSException.hpp"
#include "GAIException.hpp"

#include <sstream>
#include <cstring>

#ifndef NDEBUG
#include <iostream>
#endif

using namespace std;

namespace MiniSocket {

// UDPSocket
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

// UDPClientSocket
UDPClientSocket::UDPClientSocket(const SocketAddress &foreignAddress)
{
    int domain = foreignAddress.getSockaddr()->sa_family;
    createSocket(domain, SOCK_STREAM, 0);
    connect(foreignAddress);
}

void UDPClientSocket::disconnect()
{
    sockaddr_in nullAddr;
    memset(&nullAddr, 0, sizeof(nullAddr));
    nullAddr.sin_family = AF_UNSPEC;

    if (::connect(sockDesc_, (sockaddr *) &nullAddr, sizeof (nullAddr)) != 0) {
#ifdef WIN32
        if (WSAGetLastError() != WSAEAFNOSUPPORT)
#else
        if (errno != EAFNOSUPPORT)
#endif
            sys_error("Disconnect failed (connect())");
    }
}

// DNSResolver 
DNSResolver::Iterator DNSResolver::query(const char *host, const char *serv, addrinfo *hints)
{
    addrinfo *res;
    int n;
    if ( (n = getaddrinfo(host, serv, hints, &res)) != 0) {
        gai_error("Resolve DNS query failed (getaddrinfo())", n);
    }

    auto deleter = [](addrinfo *ptr) {
#ifndef NDEBUG
                        cout << "freeaddrinfo called" << endl;
#endif
                        freeaddrinfo(ptr);
                    };

    return Iterator(std::shared_ptr<addrinfo>(res, deleter));
}

DNSResolver::Iterator DNSResolver::query(const char *host, const char *serv, 
        TransportLayerType trans_type)
{
    addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = (int) trans_type;

    return query(host, serv, &hints);
}

DNSResolver::Iterator DNSResolver::query(const char *host, const char *serv, 
        NetworkLayerType net_type, TransportLayerType trans_type)
{
    addrinfo hints = {};
    hints.ai_family = (int) net_type;
    hints.ai_socktype = (int) trans_type;

    return query(host, serv, &hints);
}

}   // namesapce MiniSocket
