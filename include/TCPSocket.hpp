/**
 * @file TCPSocket.hpp
 * @brief 面向流的Socket
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-07
 */
#ifndef MINI_SOCKET_TCP_SOCKET_INC
#define MINI_SOCKET_TCP_SOCKET_INC

#include <iosfwd>
#include "CommunicatingSocket.hpp"

namespace MiniSocket {

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

}   // MiniSocket

#endif
