#include "Socket.hpp"
#include "SYSException.hpp"

#if defined WIN32 or defined _WIN32

#ifndef WSVERS
#define WSVERS MAKEWORD(2, 0)
#endif

namespace mini_socket {

class _WSAStartupHolder_ {
public:
    _WSAStartupHolder_()
    {
        if (WSAStartup(WSVERS, &wsadata) != 0)
            sys_error("WSAStartup error");
#ifndef NDEBUG
        cout << "WSAStartup ok" << endl;
#endif
    } 

    ~_WSAStartupHolder_()
    {
        WSACleanup();
#ifndef NDEBUG
        cout << "WSACleanup ok" << endl;
#endif
    }

private:
    WSADATA wsadata;
};

shared_ptr<_WSAStartupHolder_> makeSharedWSAStartupHolder()
{
    static shared_ptr<_WSAStartupHolder_> shared_holder(new _WSAStartupHolder_);
    return shared_holder;
}

_WSAStartupSharedHolder_::_WSAStartupSharedHolder_()
{
#ifndef NDEBUG
    cout << "_WSAStartupSharedHolder_::_WSAStartupSharedHolder_()" << endl;
#endif
    sharedHolder_ = makeSharedWSAStartupHolder();
}

_WSAStartupSharedHolder_::~_WSAStartupSharedHolder_()
{
#ifndef NDEBUG
    cout << "_WSAStartupSharedHolder_::~_WSAStartupSharedHolder_()" << endl;
#endif
}

}   // namespace mini_socket

#else
#include <unistd.h>
#endif

namespace mini_socket {

Socket::~Socket()
{
    if (isOpened())
        close();
}

void Socket::open(NetworkLayerType version, TransportLayerType type)
{
    createSocket(static_cast<int>(version), static_cast<int>(type), 0);   
}

bool Socket::open(NetworkLayerType version, TransportLayerType type, SocketError &ec)
{
    return createSocket(static_cast<int>(version), static_cast<int>(type), 0, ec);
}

void Socket::close()
{
#if defined WIN32 or defined _WIN32
  closesocket(sockDesc_);
#else
  shutdown(sockDesc_, SHUT_RD);
  ::close(sockDesc_);
#endif
  sockDesc_ = INVALID_SOCKET;
}

bool Socket::isOpened() const
{
    return sockDesc_ != INVALID_SOCKET; 
}

SocketAddress Socket::getLocalAddress() const
{
    sockaddr_storage addr;
    socklen_t addrLen = sizeof(addr);

    if (getsockname(sockDesc_, (sockaddr *) &addr, &addrLen) != 0) {
        sys_error("Fetch of local address failed (getsockname())");
    }

    return SocketAddress((sockaddr *)&addr, addrLen);
}

void Socket::createSocket(int domain, int type, int protocol)
{
    if (isOpened())
        close();

    sockDesc_ = socket(domain, type, protocol);
    if (!isOpened()) {
        sys_error("Can't create socket");
    }
}

bool Socket::createSocket(int domain, int type, int protocol, SocketError &ec)
{
    if (isOpened())
        close();

    sockDesc_ = socket(domain, type, protocol);
    if (!isOpened()) {
        get_last_sys_error(ec);
        return false;
    }
    return true;
}

void Socket::bind(const SocketAddress &localAddress)
{
    if (::bind(sockDesc_, localAddress.getSockaddr(), localAddress.getSockaddrLen()) != 0) {
        sys_error("bind error");
    }
}

}   // namespace mini_socket
