#include "DNSResolver.hpp"
#include "GAIException.hpp"

#ifndef NDEBUG
#include <iostream>
#endif

namespace mini_socket {

using std::shared_ptr;

// DNSResolver 
DNSResolver::Iterator DNSResolver::query(const char *host, const char *serv, addrinfo *hints)
{
    addrinfo *res;
    int n;
    if ( (n = getaddrinfo(host, serv, hints, &res)) != 0) {
        gai_error("Resolve DNS query failed (getaddrinfo())", n);
    }

    auto deleter = [](addrinfo *ptr) {
#ifndef NDEBUG
						std::cout << "freeaddrinfo called" << std::endl;
#endif
                        freeaddrinfo(ptr);
                    };

    return Iterator(shared_ptr<addrinfo>(res, deleter));
}

DNSResolver::Iterator DNSResolver::query(const char *host, const char *serv, 
        TransportLayerType trans_type)
{
    addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = (int) trans_type;

    return query(host, serv, &hints);
}

DNSResolver::Iterator DNSResolver::query(const char *host, const char *serv, 
        NetworkLayerType net_type, TransportLayerType trans_type)
{
    addrinfo hints = {};
    hints.ai_family = (int) net_type;
    hints.ai_socktype = (int) trans_type;

    return query(host, serv, &hints);
}

}   // namesapce mini_socket
