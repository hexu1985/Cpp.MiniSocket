/**
 * @file DNSResolver.hpp
 * @brief DNS解析类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-07
 */
#ifndef MINI_SOCKET_DNS_RESOLVER_INC
#define MINI_SOCKET_DNS_RESOLVER_INC

#include <memory>
#include "SocketCommon.hpp"
#include "SocketAddressView.hpp"

namespace mini_socket {

/**
 * @brief DNS解析类
 */
class DNSResolver {
public:
    /**
     * @brief DNS解析结果迭代器
     */
    struct ResultIterator {
        addrinfo *res = 0;

        ResultIterator(): res(nullptr)
        {
        }

        ResultIterator(addrinfo *res): res(res)
        {
        }

        void next()
        {
            res = res->ai_next;
        }

        SocketAddressView operator *() const
        {
            return SocketAddressView(res->ai_addr, res->ai_addrlen);
        }

        ResultIterator &operator ++()
        {
            next();
            return *this;
        }

        ResultIterator operator ++(int)
        {
            ResultIterator tmp(*this);
            next();
            return tmp;
        }

        bool operator ==(const ResultIterator &rhs) const
        {
            return this->res == rhs.res;
        }

        bool operator !=(const ResultIterator &rhs) const
        {
            return !(*this == rhs);
        }
    };

    /**
     * @brief DNS解析结果集
     */
    struct ResultRange {
        std::shared_ptr<addrinfo> ressave;

        ResultRange(std::shared_ptr<addrinfo> ressave): ressave(ressave) {}

        ResultIterator begin() const { return ResultIterator(ressave.get()); }
        ResultIterator end() const { return ResultIterator(nullptr); }
    };

    /**
     * @brief DNS解析请求, 可以指定host, service和传输层协议类型
     *
     * @param host 被解析的主机名
     * @param serv 被解析的服务名
     * @param trans_type 传输层协议类型: TCP or UDP
     *
     * @return 解析结果迭代器
     */
    ResultRange query(const char *host, const char *serv, TransportLayerType trans_type); 

    /**
     * @brief DNS解析请求, 可以指定host, service, 网络层协议类型和传输层协议类型
     *
     * @param host 被解析的主机名
     * @param serv 被解析的服务名
     * @param net_type 网络层协议类型: IPv4 or IPv6
     * @param trans_type 传输层协议类型: TCP or UDP
     *
     * @return 解析结果迭代器
     */
    ResultRange query(const char *host, const char *serv, NetworkLayerType net_type, TransportLayerType trans_type); 

private:
    std::shared_ptr<addrinfo> query(const char *host, const char *serv, addrinfo *hints); 
};

}   // mini_socket

#endif
