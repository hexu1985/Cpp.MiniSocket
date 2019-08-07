#include "SocketCommon.hpp"
#include <sstream>

namespace MiniSocket {

using std::ostringstream;
using std::string;
using std::tuple;

tuple<string, uint16_t> get_address_port(const sockaddr *sa, socklen_t salen)
{
	static const tuple<string, uint16_t> null_result;
	char str[128] = {0};

	switch (sa->sa_family) {
	case AF_INET: {
		sockaddr_in	*sin = (sockaddr_in *) sa;

		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return null_result;

        return make_tuple(string(str), ntohs(sin->sin_port));
	}

	case AF_INET6: {
		sockaddr_in6	*sin6 = (sockaddr_in6 *) sa;

		if (inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL)
			return null_result;

        return make_tuple(string(str), ntohs(sin6->sin6_port));
	}

	default:
        return null_result;
	}
    return null_result;
}

string to_string(const sockaddr *sa, socklen_t salen)
{
    static const string null_result;

    string address;
    uint16_t port;
    tie(address, port) = get_address_port(sa, salen);
    if (address.empty())
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

MiniSocket::NetworkLayerType get_network_layer_type(const sockaddr *sa, socklen_t salen)
{
    MiniSocket::NetworkLayerType type = MiniSocket::NetworkLayerType::UNKNOWN; 
	switch (sa->sa_family) {
	case AF_INET:
        type = MiniSocket::NetworkLayerType::IPv4;
        break;
	case AF_INET6:
        type = MiniSocket::NetworkLayerType::IPv6;
        break;
	default:
        break;
	}

    return type;
}

}   // namespace MiniSocket
