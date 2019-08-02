#ifndef MINI_SOCKET_SOCKET_EXCEPTION_INC
#define MINI_SOCKET_SOCKET_EXCEPTION_INC

#include <stdexcept>

namespace MiniSocket {

enum class ErrorCodeType {
    UNKNOWN = UINT16_MAX,   /**< 未知错误 */
    ERRNO = 1,              /**< E* */
    GAI_ERRNO = 2,          /**< EAI_* */
};

struct ErrorCode {
    int type = (int) ErrorCodeType::UNKNOWN;
    int value = 0;

    ErrorCode(int type_, int value_): type(type_), value(value_) {}
};

/**
 * @brief MiniSocket库的异常类, 表征任何Socket相关接口的异常
 */
class Exception : public std::exception {
public:
    Exception(const std::string &message, const ErrorCode &error): 
        message_(message), error_(error) {}

    ~Exception();

    const ErrorCode &getErrorCode() const { return error_; }

    const char* what() const noexcept override;

protected:
    std::string message_;
    ErrorCode error_;
};

class SocketException: public Exception {
public:
    SocketException(const std::string &message, int error = 0);
    ~SocketException();
};

class GAIException: public Exception {
public:
    GAIException(int error);
    ~GAIException();
};

void throw_socket_exception(const std::string &message);
void throw_gai_exception(int error);

}   // MiniSocket

#endif
