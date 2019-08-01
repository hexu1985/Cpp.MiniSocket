#ifndef MINI_SOCKET_SOCKET_EXCEPTION_INC
#define MINI_SOCKET_SOCKET_EXCEPTION_INC

#include <stdexcept>

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

}   // MiniSocket

#endif
