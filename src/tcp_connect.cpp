#include "tcp_connect.hpp"

using namespace std;

namespace MiniSocket {

shared_ptr<TCPSocket> tcp_connect(const char *host, const char *serv)
{
    DNSResolver resolver;
    auto iter = resolver.query(host, serv, TransportLayerType::TCP);
    while (iter.hasNext()) {
        SocketAddressView addr = iter.next();
        shared_ptr<TCPSocket> sock(new TCPSocket);
        if (!sock->open(addr.getNetworkLayerType(), TransportLayerType::TCP, nothrow))  
            continue;

        if (!sock->connect(addr, nothrow))
            continue;

        return sock;
    }

    throw SocketException("tcp connect error");
}

}   // namespace MiniSocket
