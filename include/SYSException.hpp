/**
 * @file SYSException.hpp
 * @brief 系统类型的异常
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-07
 */
#ifndef SOCKET_SYS_EXCEPTION_INC
#define SOCKET_SYS_EXCEPTION_INC

#include "SocketException.hpp"

namespace MiniSocket {

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

}   // MiniSocket

#endif
