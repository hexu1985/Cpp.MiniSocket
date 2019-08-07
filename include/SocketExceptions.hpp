/**
 * @file SocketException.hpp
 * @brief socket相关异常类的定义
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-02
 */
#ifndef MINI_SOCKET_SOCKET_EXCEPTION_INC
#define MINI_SOCKET_SOCKET_EXCEPTION_INC

#include <stdexcept>
#include "SocketError.hpp"

namespace MiniSocket {

/**
 * @brief MiniSocket库的所有异常类的基类, 表征任何Socket相关接口的异常
 */
class SocketException : public std::runtime_error {
public:
    /**
     * @brief 创建SocketExecption对象
     *
     * @param message 异常消息
     * @param error 错误码
     */
    SocketException(const std::string &message, const ErrorCode &error);

    /**
     * @brief 析构SocketException对象
     */
    ~SocketException();

    /**
     * @brief 获取错误码
     *
     * @return 错误码
     */
    const ErrorCode &getErrorCode() const { return error_; }

protected:
    ErrorCode error_;   // 统一错误码
};

/**
 * @brief 系统类型的异常
 */
class SYSException: public SocketException {
public:
    /**
     * @brief 创建SYSExecption对象
     *
     * @param message 异常消息
     * @param error 错误码
     */
    SYSException(const std::string &message, int error = 0);

    /**
     * @brief 析构SYSException对象
     */
    ~SYSException();
};

/**
 * @brief 获取地址信息类型的异常
 */
class GAIException: public SocketException {
public:
    /**
     * @brief 创建GAIExecption对象
     *
     * @param message 异常消息
     * @param error 错误码
     */
    GAIException(const std::string &message, int error = 0);

    /**
     * @brief 析构GAIException对象
     */
    ~GAIException();
};

/**
 * @brief 抛出系统类型异常
 *
 * @param message 异常消息
 * @param error 错误码
 */
[[ noreturn ]] void sys_error(const std::string &message, int error);

/**
 * @brief 抛出系统类型异常
 *
 * @param message 异常消息
 *
 * @note error使用errno全局变量值 
 */
[[ noreturn ]] void sys_error(const std::string &message);

/**
 * @brief 抛出获取地址信息类型的异常
 *
 * @param message 异常消息
 * @param error 错误码
 */
[[ noreturn ]] void gai_error(const std::string &message, int error);

}   // MiniSocket

#endif
