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

#if defined WIN32 or defined _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#endif

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

namespace MiniSocket {

/**
 * @brief 面向连接的Socket
 */
class CommunicatingSocket : public Socket {
public:
    CommunicatingSocket() = default; 

    /**
     * @brief 连接到远端地址
     *
     * @param foreignAddress 远端地址
     *
     * @note 连接失败会抛出SocketException异常
     */
    void connect(const SocketAddress &foreignAddress);
    void connect(const SocketAddressView &foreignAddress);

    /**
     * @brief 连接到远端地址
     *
     * @param foreignAddress 远端地址
     * @param[out] ec 返回错误码
     *
     * @return 如果成功返回true; 否则返回false, 并设置错误码.
     */
    bool connect(const SocketAddress &foreignAddress, ErrorCode &ec);
    bool connect(const SocketAddressView &foreignAddress, ErrorCode &ec);

    /**
     * @brief 发送数据
     *
     * @param buffer 要发送的数据内容
     * @param bufferLen 数据长度
     *
     * @return 返回发送出的数据长度
     *
     * @note 可能会抛出SocketException异常, 发送长度也可能会小于bufferLen
     */
    int send(const char *buffer, int bufferLen); 

    /**
     * @brief 接收数据
     *
     * @param buffer 接收数据缓存地址
     * @param bufferLen 缓存长度
     *
     * @return 接收数据长度
     *
     * @note 可能会抛出SocketException异常
     */
    int recv(char *buffer, int bufferLen); 

    /**
     * @brief 获取已连接成功的对端地址
     *
     * @return 对端socket地址
     */
    SocketAddress getForeignAddress() const;
};

/**
 * @brief 面向流的Socket
 */
class TCPSocket : public CommunicatingSocket {
public:
    TCPSocket() = default;

    /**
     * @brief 创建一个面向流的Socket, 并connect到服务器端地址
     *
     * @param foreignAddress 服务器端地址
     *
     * @note 可能会抛出SocketException异常
     */
    TCPSocket(const SocketAddress &foreignAddress); 

    /**
     * @brief 发送所有数据
     *
     * @param buffer 要发送数据的内容
     * @param bufferLen 数据长度
     *
     * @note 可能会抛出SocketException异常
     */
    void sendAll(const char *buffer, int bufferLen); 

    /**
     * @brief 获取当前socket的iostream子类
     *
     * @return iostream子类
     */
    std::iostream &getStream(); 

private:
    friend class TCPServerSocket;
    TCPSocket(SOCKET sockDesc);

    std::iostream *myStream_ = 0;
    std::streambuf *myStreambuf_ = 0;
};

/**
 * @brief 面向流的Socket的Server端
 */
class TCPServerSocket : public Socket {
public:
    TCPServerSocket() = default;

    /**
     * @brief 创建一个面向流的Socket的Server端, 绑定本地Socket地址, 并监听
     *
     * @param localAddress 绑定本地地址
     */
    TCPServerSocket(const SocketAddress &localAddress); 

    /**
     * @brief 设置排队队列长度
     *
     * @param backlog 排队队列长度 
     *
     * @note 当创建一个默认构造TCPServerSocket的时候, 才需要手动调用这个接口
     */
	void listen(int backlog);

    /**
     * @brief 从已完成连接队列返回一下个已连接socket
     *
     * @return 已连接的TCPSocket对象
     */
    std::shared_ptr<TCPSocket> accept();
};

/**
 * @brief 用户报文Socket
 */
class UDPSocket : public Socket {
public:
    UDPSocket() = default;

    /**
     * @brief 创建一个绑定本地Socket地址UDPSocket
     *
     * @param localAddress 本地Socket地址
     */
    UDPSocket(const SocketAddress &localAddress); 

    /**
     * @brief 向指定socket地址发送数据
     *
     * @param buffer 要发送的数据内容
     * @param bufferLen 数据长度
     * @param foreignAddress 远端地址
     *
     * @return 已发送数据长度
     */
    int sendTo(const char *buffer, int bufferLen,
            const SocketAddress &foreignAddress);

    /**
     * @brief 接收数据
     *
     * @param buffer 接收数据缓存地址
     * @param bufferLen 缓存长度
     * @param sourceAddress 发送端地址
     *
     * @return 接收数据长度
     */
    int recvFrom(char *buffer, int bufferLen,
            SocketAddress &sourceAddress); 
};

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
