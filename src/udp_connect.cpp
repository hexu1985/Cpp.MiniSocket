#include "udp_connect.hpp"
#include "SYSException.hpp"

namespace MiniSocket {

std::shared_ptr<UDPClientSocket> udp_connect(const char *host, const char *serv)
{
    DNSResolver resolver;
    ErrorCode ec;
    for (const auto &addr: resolver.query(host, serv, TransportLayerType::UDP)) {
        std::shared_ptr<UDPClientSocket> sock(new UDPClientSocket);
        if (!sock->open(addr.getNetworkLayerType(), TransportLayerType::UDP, ec))  
            continue;

        if (!sock->connect(addr, ec))
            continue;

        return sock;
    }

    sys_error("udp connect error");
}

}   // namespace MiniSocket
