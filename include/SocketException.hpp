/**
 * @file SocketException.hpp
 * @brief mini_socket库的所有异常类的基类, 表征任何Socket相关接口的异常
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-02
 */
#ifndef MINI_SOCKET_SOCKET_EXCEPTION_INC
#define MINI_SOCKET_SOCKET_EXCEPTION_INC

#include <stdexcept>
#include "SocketError.hpp"

namespace mini_socket {

/**
 * @brief mini_socket库的所有异常类的基类, 表征任何Socket相关接口的异常
 */
class SocketException : public std::runtime_error {
public:
    /**
     * @brief 创建SocketExecption对象
     *
     * @param message 异常消息
     * @param error 错误码
     */
    SocketException(const std::string &message, const SocketError &error);

    /**
     * @brief 析构SocketException对象
     */
    ~SocketException();

    /**
     * @brief 获取错误码
     *
     * @return 错误码
     */
    const SocketError &getSocketError() const { return error_; }

protected:
    SocketError error_;   // 统一错误码
};

}   // mini_socket

#endif
