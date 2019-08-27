/**
 * @file UDPClientSocket.hpp
 * @brief 面向连接的UDP Socket类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-07
 */
#ifndef MINI_SOCKET_UDP_CLIENT_SOCKET_INC
#define MINI_SOCKET_UDP_CLIENT_SOCKET_INC

#include "CommunicatingSocket.hpp"

namespace mini_socket {

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

}   // mini_socket

#endif
