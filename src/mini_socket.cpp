#include "mini_socket.hpp"

#include <sstream>
#include <cstring>

#ifndef NDEBUG
#include <iostream>
#endif

using namespace std;

#if defined WIN32 or defined _WIN32

#ifndef WSVERS
#define WSVERS MAKEWORD(2, 0)
#endif

namespace MiniSocket {

class _WSAStartupHolder_ {
public:
    _WSAStartupHolder_()
    {
        if (WSAStartup(WSVERS, &wsadata) != 0)
            throw SocketException("WSAStartup error");
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

}

#endif

namespace MiniSocket {

// SocketException
SocketException::SocketException(const string &message) :
    runtime_error(message) 
{
}

SocketException::SocketException(const string &message, const string &detail) :
    runtime_error(message + ": " + detail) 
{
}

// SocketAddressView
tuple<string, uint16_t> SocketAddressView::getAddressPort(const sockaddr *sa, socklen_t salen)
{
	static const tuple<string, uint16_t> null_result;
	char str[128] = {0};

	switch (sa->sa_family) {
	case AF_INET: {
		sockaddr_in	*sin = (sockaddr_in *) sa;

		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return null_result;

        return make_tuple(std::string(str), ntohs(sin->sin_port));
	}

	case AF_INET6: {
		sockaddr_in6	*sin6 = (sockaddr_in6 *) sa;

		if (inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL)
			return null_result;

        return make_tuple(std::string(str), ntohs(sin6->sin6_port));
	}

	default:
        return null_result;
	}
    return null_result;
}

string SocketAddressView::toString(const sockaddr *sa, socklen_t salen)
{
    static const std::string null_result;

    string address;
    uint16_t port;
    std::tie(address, port) = getAddressPort(sa, salen);
    if (port == 0)
        return null_result;

    ostringstream os;
    if (sa->sa_family == AF_INET6)
        os << "[";
    os << address;
    if (sa->sa_family == AF_INET6)
        os << "]";
    os << ":" << port;
    return os.str();
}

SocketAddressView::SocketAddressView(const sockaddr *addrVal, socklen_t addrLenVal) : addr_(addrVal), addrLen_(addrLenVal)
{
}

string SocketAddressView::toString() const
{
    return toString(getSockaddr(), getSockaddrLen());
}

tuple<std::string, uint16_t> SocketAddressView::getAddressPort() const
{
    return getAddressPort(getSockaddr(), getSockaddrLen());
}

const sockaddr *SocketAddressView::getSockaddr() const 
{
    return addr_;
}

socklen_t SocketAddressView::getSockaddrLen() const 
{
    return addrLen_;
}

SocketAddress::SocketAddress(): addrLen_(0)
{
    memset(&addr_, 0, sizeof(addr_));
}

// SocketAddress
SocketAddress::SocketAddress(const char *address, uint16_t port): SocketAddress()
{
    if (setAddressPort(address, port))
        return;

    ostringstream os;
    os << "the address is [" << address << "], and port is [" << port << "]";
    throw SocketException("Construct SocketAddress error", os.str());
}

SocketAddress::SocketAddress(sockaddr *addrVal, socklen_t addrLenVal):
    addrLen_(addrLenVal) 
{
    memcpy( &addr_, addrVal, addrLenVal );
}

bool SocketAddress::setAddressPort(const char *address, uint16_t port)
{
    sockaddr *sa = (sockaddr *) &addr_;

    sa->sa_family = AF_INET;
    sockaddr_in	*sin = (sockaddr_in *) sa;
    if (inet_pton(sa->sa_family, address, &sin->sin_addr) == 1) { // success
        sin->sin_port = htons(port);
        return true;
    }

    sa->sa_family = AF_INET6;
    sockaddr_in6 *sin6 = (sockaddr_in6 *) sa;
    if (inet_pton(sa->sa_family, address, &sin6->sin6_addr) == 1) { // success
        sin6->sin6_port = htons(port);
        return true;
    }

    return false;
}

string SocketAddress::toString() const
{
    return SocketAddressView::toString(getSockaddr(), getSockaddrLen());
}

tuple<std::string, uint16_t> SocketAddress::getAddressPort() const
{
    return SocketAddressView::getAddressPort(getSockaddr(), getSockaddrLen());
}

sockaddr *SocketAddress::getSockaddr() const 
{
    return (sockaddr *)&addr_;
}

socklen_t SocketAddress::getSockaddrLen() const 
{
    return addrLen_;
}

}   // namesapce MiniSocket
