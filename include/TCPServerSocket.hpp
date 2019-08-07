#ifndef MINI_SOCKET_TCP_SERVER_SOCKET_INC
#define MINI_SOCKET_TCP_SERVER_SOCKET_INC

#include "Socket.hpp"

namespace MiniSocket {

class TCPSocket;

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

}   // MiniSocket

#endif
