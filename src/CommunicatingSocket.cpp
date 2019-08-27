#include "CommunicatingSocket.hpp"
#include "SYSException.hpp"
#include "SocketAddressView.hpp"

namespace mini_socket {

// CommunicatingSocket 
void CommunicatingSocket::connect(const SocketAddress &foreignAddress)
{
    if (::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen()) != 0) {
        sys_error("connect error");
    }
}

void CommunicatingSocket::connect(const SocketAddressView &foreignAddress)
{
    if (::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen()) != 0) {
        sys_error("connect error");
    }
}

bool CommunicatingSocket::connect(const SocketAddress &foreignAddress, SocketError &ec)
{
    int ret = ::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen());
    if (ret != 0) {
        get_last_sys_error(ec);
        return false;
    }
    return true;
}

bool CommunicatingSocket::connect(const SocketAddressView &foreignAddress, SocketError &ec)
{
    int ret = ::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen());
    if (ret != 0) {
        get_last_sys_error(ec);
        return false;
    }
    return true;
}

int CommunicatingSocket::send(const char *buffer, int bufferLen)
{
    int n = ::send(sockDesc_, buffer, bufferLen, 0);
    if ( n < 0 ) {
        sys_error("Send failed (send())");
    }

    return n;
}

int CommunicatingSocket::recv(char *buffer, int bufferLen)
{
    int n = ::recv(sockDesc_, buffer, bufferLen, 0); 
    if ( n < 0 ) {
        sys_error("Receive failed (recv())");
    }

    return n;
}

SocketAddress CommunicatingSocket::getForeignAddress() const
{
    sockaddr_storage addr;
    socklen_t addrLen = sizeof(addr);

    if (getpeername(sockDesc_, (sockaddr *) &addr, &addrLen) != 0) {
        sys_error("Fetch of foreign address failed (getpeername())");
    }

    return SocketAddress((sockaddr *)&addr, addrLen);
}

}   // namesapce mini_socket
