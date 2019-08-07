/**
 * @file mini_socket.hpp
 * @brief 一个简单的跨平台的C++网络编程库
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-05-22
 *
 * @see TCP/IP Sockets in C Second Edition, Chapter 8, PracticalSocket lib \n
 * https://github.com/hexu1985/TCP.IP.Sockets.in.C
 */
#ifndef MINI_SOCKET_INC
#define MINI_SOCKET_INC

#include <cstdint>
#include <string>
#include <tuple>
#include <memory>
#include <iostream>
#include "SocketException.hpp"
#include "SocketCommon.hpp"
#include "SocketAddress.hpp"
#include "SocketAddressView.hpp"
#include "Socket.hpp"
#include "CommunicatingSocket.hpp"
#include "TCPSocket.hpp"
#include "TCPServerSocket.hpp"
#include "UDPSocket.hpp"

namespace MiniSocket {

/**
 * @brief 面向连接的UDP Socket类
 */
class UDPClientSocket: public CommunicatingSocket {
public:
    UDPClientSocket() = default;

    /**
     * @brief 创建一个面连接的UDP Socket, 并connect到服务器端地址
     *
     * @param foreignAddress 服务器端地址
     *
     * @note 可能会抛出SocketException异常
     */
    UDPClientSocket(const SocketAddress &foreignAddress); 

    /**
     * @brief 断开连接
     */
    void disconnect();
};

/**
 * @brief DNS解析类
 */
class DNSResolver {
public:
    /**
     * @brief DNS解析结果迭代器
     */
    struct Iterator {
        addrinfo *res = 0;
        std::shared_ptr<addrinfo> ressave;

        Iterator(): res(nullptr)
        {
        }

        Iterator(std::shared_ptr<addrinfo> result):
            res(result.get()), ressave(result)
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

        Iterator &operator ++()
        {
            next();
            return *this;
        }

        Iterator operator ++(int)
        {
            Iterator tmp(*this);
            next();
            return tmp;
        }

        bool operator ==(const Iterator &rhs) const
        {
            return this->res == rhs.res;
        }

        bool operator !=(const Iterator &rhs) const
        {
            return !(*this == rhs);
        }
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
    Iterator query(const char *host, const char *serv, TransportLayerType trans_type); 

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
    Iterator query(const char *host, const char *serv, NetworkLayerType net_type, TransportLayerType trans_type); 

private:
    Iterator query(const char *host, const char *serv, addrinfo *hints); 
};

/**
 * @brief 获取开始迭代器
 *
 * @param iter DNS解析结果迭代器
 *
 * @return DNS解析结果迭代器的起始
 *
 * @note 为了支持如下使用方式:
 * for (auto addr: resolver.query(host, serv, TransportLayerType::TCP)) {
 *      // process addr
 * }
 */
inline DNSResolver::Iterator begin(const DNSResolver::Iterator &iter)
{
    return DNSResolver::Iterator(iter.ressave);
}

/**
 * @brief 获取结束迭代器
 *
 * @param iter DNS解析结果迭代器
 *
 * @return DNS解析结果迭代器的结束
 *
 * @note 为了支持如下使用方式:
 * for (auto addr: resolver.query(host, serv, TransportLayerType::TCP)) {
 *      // process addr
 * }
 */
inline DNSResolver::Iterator end(const DNSResolver::Iterator &iter)
{
    return DNSResolver::Iterator();
}

}   // MiniSocket

#endif
