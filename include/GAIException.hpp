#ifndef MINI_SOCKET_GAI_EXCEPTION_INC
#define MINI_SOCKET_GAI_EXCEPTION_INC

#include "SocketException.hpp"

namespace MiniSocket {

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
 * @brief 抛出获取地址信息类型的异常
 *
 * @param message 异常消息
 * @param error 错误码
 */
[[ noreturn ]] void gai_error(const std::string &message, int error);

}   // MiniSocket

#endif
