#include "mini_socket.hpp"

#include <sstream>

using namespace std;

tuple<string, uint16_t> sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    static const tuple<string, uint16_t> null_result;
    char str[128];

	switch (sa->sa_family) {
	case AF_INET: {
		struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return null_result;

        return make_tuple(std::string(str), ntohs(sin->sin_port));
	}

	case AF_INET6: {
		struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

		if (inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, sizeof(str) - 1) == NULL)
			return null_result;

        return make_tuple(std::string(str), ntohs(sin6->sin6_port));
	}

	default:
        return null_result;
	}
    return null_result;
}

string to_string(const struct sockaddr *sa, socklen_t salen)
{
    static const std::string null_result;

    string ip;
    uint16_t port;
    std::tie(ip, port) = sock_ntop(sa, salen);
    if (port == 0)
        return null_result;

    ostringstream os;
    if (sa->sa_family == AF_INET6)
        os << "[";
    os << ip;
    if (sa->sa_family == AF_INET6)
        os << "]";
    os << ":" << port;
    return os.str();
}

