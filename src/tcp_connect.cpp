#include "tcp_connect.hpp"

namespace MiniSocket {

std::shared_ptr<TCPSocket> tcp_connect(const char *host, const char *serv)
{
    DNSResolver resolver;
    for (const auto &addr: resolver.query(host, serv, TransportLayerType::TCP)) {
        std::shared_ptr<TCPSocket> sock(new TCPSocket);
        if (!sock->open(addr.getNetworkLayerType(), TransportLayerType::TCP, std::nothrow))  
            continue;

        if (!sock->connect(addr, std::nothrow))
            continue;

        return sock;
    }

    throw SocketException("tcp connect error");
}

}   // namespace MiniSocket
