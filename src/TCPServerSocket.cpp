#include "TCPServerSocket.hpp"
#include "TCPSocket.hpp"
#include "SYSException.hpp"

using namespace std;

namespace MiniSocket {

// TCPServerSocket
TCPServerSocket::TCPServerSocket(const SocketAddress &localAddress)
{
    int domain = localAddress.getSockaddr()->sa_family;
    createSocket(domain, SOCK_STREAM, 0);
    bind(localAddress);
	const int LISTENQ = 1024;
	listen(LISTENQ);
}

void TCPServerSocket::listen(int backlog)
{
    if (::listen(sockDesc_, backlog) != 0) {
        sys_error("listen error");
    }
}

shared_ptr<TCPSocket> TCPServerSocket::accept()
{
    SOCKET newConnSD;
    if ((newConnSD = ::accept(sockDesc_, NULL, 0)) == INVALID_SOCKET) {
        sys_error("Accept failed (accept())");
    }

    return shared_ptr<TCPSocket>(new TCPSocket(newConnSD));
}

}   // namesapce MiniSocket
