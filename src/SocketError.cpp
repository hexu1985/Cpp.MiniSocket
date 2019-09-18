#include <cstring>
#include <cerrno>

#include "SocketError.hpp"
#include "SocketCommon.hpp"

namespace mini_socket {

using std::string;

int get_last_sys_error()
{
#if defined (WIN32) || defined (_WIN32)
    return WSAGetLastError();
#else
    return errno;
#endif
}

#if defined (WIN32) || defined (_WIN32)
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

string get_gai_error_str(int error)
{
    return get_sys_error_str(error);
}

#else   // not win32
string get_sys_error_str(int error)
{
    return strerror(error);
}

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

}   // namespace mini_socket
