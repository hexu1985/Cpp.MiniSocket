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

}   // namespace MiniSocket
