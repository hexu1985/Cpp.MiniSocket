#include "udp_connect.hpp"
#include "SYSException.hpp"
#include "UDPClientSocket.hpp"
#include "DNSResolver.hpp"

namespace mini_socket {

using std::shared_ptr;

shared_ptr<UDPClientSocket> udp_connect(const char *host, const char *serv)
{
    DNSResolver resolver;
    SocketError ec;
    for (const auto &addr: resolver.query(host, serv, TransportLayerType::UDP)) {
        shared_ptr<UDPClientSocket> sock(new UDPClientSocket);
        if (!sock->open(addr.getNetworkLayerType(), TransportLayerType::UDP, ec))  
            continue;

        if (!sock->connect(addr, ec))
            continue;

        return sock;
    }

    sys_error("udp connect error");
}

}   // namespace mini_socket
