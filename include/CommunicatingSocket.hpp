#ifndef MINI_SOCKET_COMMUNICATING_SOCKET_INC
#define MINI_SOCKET_COMMUNICATING_SOCKET_INC

#include "Socket.hpp"

namespace MiniSocket {

class SocketAddressView;

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
    bool connect(const SocketAddress &foreignAddress, SocketError &ec);
    bool connect(const SocketAddressView &foreignAddress, SocketError &ec);

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

}   // MiniSocket

#endif
