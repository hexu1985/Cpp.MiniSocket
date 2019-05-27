#include "udp_connect.hpp"

using namespace std;

namespace MiniSocket {

shared_ptr<UDPClientSocket> udp_connect(const char *host, const char *serv)
{
    DNSResolver resolver;
    auto iter = resolver.query(host, serv, TransportLayerType::UDP);
    while (iter.hasNext()) {
        SocketAddressView addr = iter.next();
        shared_ptr<UDPClientSocket> sock(new UDPClientSocket);
        if (!sock->open(addr.getNetworkLayerType(), TransportLayerType::UDP, nothrow))  
            continue;

        if (!sock->connect(addr, nothrow))
            continue;

        return sock;
    }

    throw SocketException("udp connect error");
}

}   // namespace MiniSocket
