#include "SocketAddressView.hpp"

using namespace std;

namespace MiniSocket {

// SocketAddressView
SocketAddressView::SocketAddressView(const sockaddr *addrVal, socklen_t addrLenVal) : addr_(addrVal), addrLen_(addrLenVal)
{
}

string SocketAddressView::toString() const
{
    return to_string(getSockaddr(), getSockaddrLen());
}

tuple<string, uint16_t> SocketAddressView::getAddressPort() const
{
    return get_address_port(getSockaddr(), getSockaddrLen());
}

const sockaddr *SocketAddressView::getSockaddr() const 
{
    return addr_;
}

socklen_t SocketAddressView::getSockaddrLen() const 
{
    return addrLen_;
}

NetworkLayerType SocketAddressView::getNetworkLayerType() const
{
    return get_network_layer_type(getSockaddr(), getSockaddrLen());
}

}   // namesapce MiniSocket
