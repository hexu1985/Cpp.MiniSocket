/**
 * @file Socket.hpp
 * @brief socket所有类的基类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-06
 */
#ifndef MINI_SOCKET_SOCKET_INC
#define MINI_SOCKET_SOCKET_INC

#include <memory>
#include "SocketCommon.hpp"
#include "SocketAddress.hpp"
#include "SocketError.hpp"

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
     * @brief 打开socket, 以SocketError方式替代SocketException
     *
     * @param version 网络层协议版本
     * @param type 传输层协议版本
     * @param[out] ec 返回错误码
     *
     * @return 如果成功返回true; 否则返回false, 并设置错误码.
     */
    bool open(NetworkLayerType version, TransportLayerType type, SocketError &ec);

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
    SOCKET sockDesc_ = INVALID_SOCKET;  // socket描述符

    Socket() = default;
    void createSocket(int domain, int type, int protocol);
    bool createSocket(int domain, int type, int protocol, SocketError &ec);
};

}   // namespace MiniSocket

#endif
