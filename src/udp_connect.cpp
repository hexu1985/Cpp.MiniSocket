#include "udp_connect.hpp"

namespace MiniSocket {

std::shared_ptr<UDPClientSocket> udp_connect(const char *host, const char *serv)
{
    DNSResolver resolver;
    for (const auto &addr: resolver.query(host, serv, TransportLayerType::UDP)) {
        std::shared_ptr<UDPClientSocket> sock(new UDPClientSocket);
        if (!sock->open(addr.getNetworkLayerType(), TransportLayerType::UDP, std::nothrow))  
            continue;

        if (!sock->connect(addr, std::nothrow))
            continue;

        return sock;
    }

    sys_error("udp connect error");
}

}   // namespace MiniSocket
