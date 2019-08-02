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

namespace MiniSocket {

enum class ErrorType: uint32_t {
    UNKNOWN = 0,        /**< 未知类型 */
    SYS = 1,            /**< 系统层: use errno */
    GAI = 2,            /**< getaddrinfo接口返回的错误码: EAI_* */
    USR = UINT16_MAX,   /**< 用户侧错误 */
};

struct ErrorCode {
    int type = (int) ErrorType::UNKNOWN;
    int value = 0;

    ErrorCode(int type_, int value_): type(type_), value(value_) {}
};

inline
ErrorCode make_sys_error(int error)
{
    return ErrorCode((int) ErrorType::SYS, error);
}

inline
ErrorCode make_gai_error(int error)
{
    return ErrorCode((int) ErrorType::GAI, error);
}

/**
 * @brief MiniSocket库的异常类, 表征任何Socket相关接口的异常
 */
class SocketException : public std::runtime_error {
public:
    SocketException(const std::string &message, const ErrorCode &error);
    ~SocketException();

    const ErrorCode &getErrorCode() const { return error_; }

protected:
    ErrorCode error_;
};

class SYSException: public SocketException {
public:
    SYSException(const std::string &message, int error = 0);
    ~SYSException();
};

class GAIException: public SocketException {
public:
    GAIException(const std::string &message, int error = 0);
    ~GAIException();
};

[[ noreturn ]] void sys_error(const std::string &message, int error);
[[ noreturn ]] void sys_error(const std::string &message);

[[ noreturn ]] void gai_error(const std::string &message, int error);

}   // MiniSocket

#endif
