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
class SocketException : public std::exception {
public:
    SocketException(const std::string &message, const ErrorCode &error): 
        message_(message), error_(error) {}

    ~SocketException();

    const ErrorCode &getErrorCode() const { return error_; }

    const char* what() const noexcept override;

protected:
    std::string message_;
    ErrorCode error_;
};

class SYSException: public SocketException {
public:
    SYSException(const std::string &message, int error = 0);
    ~SYSException();
};

class GAIException: public SocketException {
public:
    GAIException(const std::string &message, int error);
    GAIException(int error);
    ~GAIException();
};

[[ noreturn ]] void sys_error(const std::string &message, int error);
[[ noreturn ]] void sys_error(const std::string &message);
[[ noreturn ]] void gai_error(const std::string &message, int error);
[[ noreturn ]] void gai_error(int error);

}   // MiniSocket

#endif
