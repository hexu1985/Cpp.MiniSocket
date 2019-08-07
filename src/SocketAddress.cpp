#include "SocketAddress.hpp"
#include "SocketExceptions.hpp"

#include <sstream>

using namespace std;

namespace MiniSocket {

SocketAddress::SocketAddress(const char *address, uint16_t port): SocketAddress()
{
    if (setAddressPort(address, port))
        return;

    ostringstream os;
    os << "Construct SocketAddress error: the address is [" << address << "], and port is [" << port << "]";
    sys_error(os.str());
}

SocketAddress::SocketAddress(sockaddr *addrVal, socklen_t addrLenVal):
    addrLen_(addrLenVal) 
{
    memcpy( &addr_, addrVal, addrLenVal );
}

bool SocketAddress::setAddressPort(const char *address, uint16_t port)
{
    sockaddr *sa = (sockaddr *) &addr_;

    sa->sa_family = AF_INET;
    sockaddr_in	*sin = (sockaddr_in *) sa;
    if (inet_pton(sa->sa_family, address, &sin->sin_addr) == 1) { // success
        sin->sin_port = htons(port);
        return true;
    }

    sa->sa_family = AF_INET6;
    sockaddr_in6 *sin6 = (sockaddr_in6 *) sa;
    if (inet_pton(sa->sa_family, address, &sin6->sin6_addr) == 1) { // success
        sin6->sin6_port = htons(port);
        return true;
    }

    return false;
}

string SocketAddress::toString() const
{
    return to_string(getSockaddr(), getSockaddrLen());
}

tuple<string, uint16_t> SocketAddress::getAddressPort() const
{
    return get_address_port(getSockaddr(), getSockaddrLen());
}

sockaddr *SocketAddress::getSockaddr() const 
{
    return (sockaddr *)&addr_;
}

socklen_t SocketAddress::getSockaddrLen() const 
{
    return addrLen_;
}

NetworkLayerType SocketAddress::getNetworkLayerType() const
{
    return get_network_layer_type(getSockaddr(), getSockaddrLen());
}

}   // namesapce MiniSocket
