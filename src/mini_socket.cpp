#include "mini_socket.hpp"

#include <sstream>

using namespace std;

namespace MiniSocket {

tuple<string, uint16_t> sock_ntop(const sockaddr *sa, socklen_t salen)
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

		if (inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, sizeof(str) - 1) == NULL)
			return null_result;

        return make_tuple(std::string(str), ntohs(sin6->sin6_port));
	}

	default:
        return null_result;
	}
    return null_result;
}

string to_string(const sockaddr *sa, socklen_t salen)
{
    static const std::string null_result;

    string address;
    uint16_t port;
    std::tie(address, port) = sock_ntop(sa, salen);
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

SocketException::SocketException(const string &message) :
    runtime_error(message) 
{
}

SocketException::SocketException(const string &message, const string &detail) :
    runtime_error(message + ": " + detail) 
{
}

SocketAddress::SocketAddress(): addrLen_(0)
{
    memset(&addr_, 0, sizeof(addr_));
}

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

}   // namesapce MiniSocket
