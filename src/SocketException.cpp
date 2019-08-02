#if defined WIN32 or defined _WIN32
#include <windows.h>
#endif

#include <errno.h>

#include "SocketException.hpp"

using namespace std;

namespace MiniSocket {

Exception::~Exception()
{
}

const char *Exception::what() const noexcept
{
    return message_.c_str();
}

namespace {

inline
ErrorCode make_errno_error(int error)
{
    return ErrorCode((int) ErrorCodeType::ERRNO, error);
}

inline
ErrorCode make_gai_error(int error)
{
    return ErrorCode((int) ErrorCodeType::GAI_ERRNO, error);
}

#if defined WIN32 or defined _WIN32
string get_errno_error_str(int error)
{
    HLOCAL hlocal = NULL;
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL,
        error, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        (LPTSTR) &hlocal, 0, NULL);
    std::string err_msg((const char *)hlocal);
    LocalFree(hlocal);

    return err_msg;
}

inline
string get_gai_error_str(int error)
{
    return get_errno_error_str(error);
}

#else   // not win32
string get_errno_error_str(int error)
{
    const int BUF_LEN = 256;
    char buf[BUF_LEN] = {0};
    strerror_r(error, buf, BUF_LEN);
    buf[BUF_LEN-1] = 0;
    return string(buf);
}

inline
string get_gai_error_str(int error)
{
    return gai_strerror(error);
}

#endif

}   // namespace {

SocketException::SocketException(const string &message, int error):
    Exception(message, make_errno_error(error))
{
    if (error != 0) {
        message_ += ": ";
        message_ += get_errno_error_str(error);
    }
}

SocketException::~SocketException()
{
}

GAIException::GAIException(int error):
    Exception("getaddrinfo error", make_gai_error(error))
{
    if (error != 0) {
        message_ += ": ";
        message_ += get_gai_error_str(error);
    }
}

GAIException::~GAIException()
{
}

void throw_socket_exception(const std::string &message)
{
#if defined WIN32 or defined _WIN32
    throw SocketException(message, WSAGetLastError());
#else
    throw SocketException(message, errno);
#endif
}

void throw_gai_exception(int error)
{
#if defined WIN32 or defined _WIN32
    throw GAIException(WSAGetLastError());
#else
    throw GAIException(error);
#endif
}

}   // namespace MiniSocket
