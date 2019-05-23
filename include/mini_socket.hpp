/**
 * @file mini_socket.hpp
 * @brief 一个简单的跨平台的C++网络编程库
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-05-22
 */
#ifndef PNL_MINI_SOCKET_INC
#define PNL_MINI_SOCKET_INC

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
#include <stdexcept>
#include <memory>
#include <iostream>

#if defined WIN32 or defined _WIN32
namespace MiniSocket {

class _WSAStartupHolder_;

class _WSAStartupSharedHolder_ {
public:
    _WSAStartupSharedHolder_(); 
    ~_WSAStartupSharedHolder_(); 

private:
    std::shared_ptr<_WSAStartupHolder_> sharedHolder_;
};

static _WSAStartupSharedHolder_ _wsa_startup_shared_holder_;

}   // namespace MiniSocket
#else   // not WIN32

typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)

#endif

namespace MiniSocket {

/**
 * @brief MiniSocket库的异常类, 表征任何Socket相关接口的异常
 */
class SocketException : public std::runtime_error {
public:
    /**
     * @brief 构造一个SocketException对象
     *
     * @param message 异常基本信息
     */
  SocketException(const std::string &message); 
  
  /**
   * @brief 构造一个SocketException对象
   *
   * @param message 异常基本信息
   * @param detail 异常详细信息
   */
  SocketException(const std::string &message, const std::string &detail);
};

/**
 * @brief 封装Socket地址的引用的类, 同时支持IPv4和IPv6
 * @note 该类并不管理指向的sockaddr的内存
 */
class SocketAddressView {
public:
    /**
     * @brief 从sockaddr指针创建一个SocketAddressView
     *
     * @param addrVal 指向sockaddr的指针
     * @param addrLenVal 实际sockaddr的长度
     */
    SocketAddressView(const sockaddr *addrVal = NULL, socklen_t addrLenVal = 0);

    /**
     * @brief 将SocketAddressView转换成可打印格式
     *
     * @return ipv4: xxx.xxx.xxx.xxx:port, ipv6: [xxx:xxx:...:xxx]:port
     */
    std::string toString() const;

    /**
     * @brief 获取可打印地址和port号
     *
     * @return 包含地址和端口号的元组
     */
    std::tuple<std::string, uint16_t> getAddressPort() const;

    /**
     * @brief 转换成sockaddr指针
     *
     * @return 指向sockaddr的指针
     */
    const sockaddr *getSockaddr() const; 

    /**
     * @brief 获取sockaddr实际的长度
     *
     * @return sockaddr长度
     */
    socklen_t getSockaddrLen() const; 

private:
    const sockaddr *addr_ = 0;
    socklen_t addrLen_ = 0;
};

/**
 * @brief 封装Socket地址的类, 同时支持IPv4和IPv6
 */
class SocketAddress {
public:
    /**
     * @brief 构造一个空的SocketAddress
     */
    SocketAddress(); 

    /**
     * @brief 根据指定ip+port构造一个SocketAddress
     *
     * @param address ip地址的字符串
     * @param port 端口号
     *
     * @note 如果地址无效, 会抛出SocketException异常
     */
    SocketAddress(const char *address, uint16_t port); 

    /**
     * @brief 从sockaddr指针转换成一个SocketAddress
     *
     * @param addrVal 指向sockaddr的指针
     * @param addrLenVal 实际sockaddr的长度
     */
    SocketAddress(sockaddr *addrVal, socklen_t addrLenVal);

    /**
     * @brief 根据指定ip+port重新设置一个SocketAddress
     *
     * @param address ip地址的字符串
     * @param port 端口号
     *
     * @return 如果是有效地址, 返回true; 否则返回false
     */
    bool setAddressPort(const char *address, uint16_t port); 

    /**
     * @brief 将SocketAddress转换成可打印格式
     *
     * @return ipv4: xxx.xxx.xxx.xxx:port, ipv6: [xxx:xxx:...:xxx]:port
     */
    std::string toString() const;

    /**
     * @brief 获取可打印地址和port号
     *
     * @return 包含地址和端口号的元组
     */
    std::tuple<std::string, uint16_t> getAddressPort() const;

    /**
     * @brief 转换成sockaddr指针
     *
     * @return 指向sockaddr的指针
     */
    sockaddr *getSockaddr() const; 

    /**
     * @brief 获取sockaddr实际的长度
     *
     * @return sockaddr长度
     */
    socklen_t getSockaddrLen() const; 

private:
    sockaddr_storage addr_ = {};
    socklen_t addrLen_ = sizeof (sockaddr_storage);
};

/// IP版本号
enum class NetworkLayerType {
    IPv4 = AF_INET,     /**< IPv4版本 */
    IPv6 = AF_INET6,    /**< IPv6版本 */
};

/// 传输层协议类型
enum class TransportLayerType {
    TCP = SOCK_STREAM,  /**< TCP协议 */
    UDP = SOCK_DGRAM,   /**< UDP协议 */
};

/**
 * @brief 封装socket文件描述符的类, 所有具体socket功能类的基类
 */
class Socket {
public:
    /**
     * @brief 虚析构函数函数, 子类根据需要override
     */
    virtual ~Socket();

    /**
     * @brief 打开socket
     *
     * @param version 网络层协议版本
     * @param type 传输层协议版本
     */
    void open(NetworkLayerType version, TransportLayerType type);

    /**
     * @brief 关闭socket
     */
    void close();

    /**
     * @brief 判断当前socket是否已打开
     *
     * @return 如果已打开, 返回true; 否则返回false
     */
    bool isOpened() const;

    /**
     * @brief 获取当前socket的本地端地址
     *
     * @return socket地址
     */
    SocketAddress getLocalAddress() const;

    /**
     * @brief 绑定当前socket的本地端地址
     *
     * @param localAddress 本地端地址
     */
    void bind(const SocketAddress &localAddress);

private:
    Socket(const Socket &sock) = delete;
    void operator=(const Socket &sock) = delete;

protected:
    SOCKET sockDesc_ = INVALID_SOCKET;

    Socket() = default;
    void createSocket(int domain, int type, int protocol);
};

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

    /**
     * @brief 连接到远端地址
     *
     * @param foreignAddress 远端地址
     * @param nothrow_value 传入std::nothrow
     *
     * @return 如果连接成功, 返回true; 否则返回false
     */
    bool connect(const SocketAddress &foreignAddress, const std::nothrow_t &nothrow_value);

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

// TCPSocket
class TCPSocket : public CommunicatingSocket {
public:
    TCPSocket() = default;
    TCPSocket(const SocketAddress &foreignAddress); 

    void sendAll(const char *buffer, int bufferLen); 

    std::iostream &getStream(); 

private:
    friend class TCPServerSocket;
    TCPSocket(SOCKET sockDesc);

    std::iostream *myStream_ = 0;
    std::streambuf *myStreambuf_ = 0;
};

// TCPServerSocket 
class TCPServerSocket : public Socket {
public:
    TCPServerSocket(const SocketAddress &localAddress); 

	void listen(int backlog);

    std::shared_ptr<TCPSocket> accept();
};

// UDPSocket
class UDPSocket : public Socket {
public:
    UDPSocket() = default;
    UDPSocket(const SocketAddress &localAddress); 

    int sendTo(const char *buffer, int bufferLen,
            const SocketAddress &foreignAddress);

    int recvFrom(char *buffer, int bufferLen,
            SocketAddress &foreignAddress); 
};

}   // MiniSocket

#endif
