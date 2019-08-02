#if defined WIN32 or defined _WIN32
#include <windows.h>
#endif

#include <errno.h>

#include "SocketException.hpp"

using namespace std;

namespace MiniSocket {

SocketException::SocketException(const string &message, const ErrorCode &error): 
    runtime_error(message), error_(error) 
{
}

SocketException::~SocketException()
{
}

namespace {

#if defined WIN32 or defined _WIN32
string get_sys_error_str(int error)
{
    HLOCAL hlocal = NULL;
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL,
        error, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        (LPTSTR) &hlocal, 0, NULL);
    string err_msg((const char *)hlocal);
    LocalFree(hlocal);

    return err_msg;
}

inline
string get_gai_error_str(int error)
{
    return get_sys_error_str(error);
}

#else   // not win32
string get_sys_error_str(int error)
{
    return strerror(error);
}

inline
string get_gai_error_str(int error)
{
    return gai_strerror(error);
}

#endif

string get_sys_error_str(const string &title, int error)
{
    string message(title);
    if (error != 0) {
        if (!title.empty())
            message += ": ";
        message += get_sys_error_str(error);
    }
    return message;
}

string get_gai_error_str(const string &title, int error)
{
    string message(title);
    if (error != 0) {
        if (!title.empty())
            message += ": ";
        message += get_gai_error_str(error);
    }
    return message;
}

}   // namespace {

SYSException::SYSException(const string &message, int error):
    SocketException(get_sys_error_str(message, error), make_sys_error(error))
{
}

SYSException::~SYSException()
{
}

GAIException::GAIException(const string &message, int error):
    SocketException(get_sys_error_str(message, error), make_gai_error(error))
{
}

GAIException::~GAIException()
{
}

void sys_error(const string &message, int error)
{
    throw SYSException(message, error);
}

void sys_error(const string &message)
{
#if defined WIN32 or defined _WIN32
    throw SYSException(message, WSAGetLastError());
#else
    throw SYSException(message, errno);
#endif
}

void gai_error(const string &message, int error)
{
#if defined WIN32 or defined _WIN32
    throw GAIException(message, WSAGetLastError());
#else
    throw GAIException(message, error);
#endif
}

}   // namespace MiniSocket
