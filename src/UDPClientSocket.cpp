#include <cstring>

#include "UDPClientSocket.hpp"
#include "SYSException.hpp"

namespace MiniSocket {

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

}   // namesapce MiniSocket
