#include "tcp_connect.hpp"
#include "SYSException.hpp"
#include "TCPSocket.hpp"
#include "DNSResolver.hpp"

namespace mini_socket {

using std::shared_ptr;

shared_ptr<TCPSocket> tcp_connect(const char *host, const char *serv)
{
    DNSResolver resolver;
    SocketError ec;
    for (const auto &addr: resolver.query(host, serv, TransportLayerType::TCP)) {
        shared_ptr<TCPSocket> sock(new TCPSocket);
        if (!sock->open(addr.getNetworkLayerType(), TransportLayerType::TCP, ec))  
            continue;

        if (!sock->connect(addr, ec))
            continue;

        return sock;
    }

    sys_error("tcp connect error");
}

}   // namespace mini_socket
