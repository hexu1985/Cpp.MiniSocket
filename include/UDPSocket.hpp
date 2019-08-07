/**
 * @file UDPSocket.hpp
 * @brief 用户报文Socket
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-07
 */
#ifndef MINI_SOCKET_UDP_SOCKET_INC
#define MINI_SOCKET_UDP_SOCKET_INC

#include "Socket.hpp"

namespace MiniSocket {

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

}   // MiniSocket

#endif
