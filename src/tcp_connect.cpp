#include "tcp_connect.hpp"

namespace MiniSocket {

std::shared_ptr<TCPSocket> tcp_connect(const char *host, const char *serv)
{
    DNSResolver resolver;
    ErrorCode ec;
    for (const auto &addr: resolver.query(host, serv, TransportLayerType::TCP)) {
        std::shared_ptr<TCPSocket> sock(new TCPSocket);
        if (!sock->open(addr.getNetworkLayerType(), TransportLayerType::TCP, ec))  
            continue;

        if (!sock->connect(addr, ec))
            continue;

        return sock;
    }

    sys_error("tcp connect error");
}

}   // namespace MiniSocket
