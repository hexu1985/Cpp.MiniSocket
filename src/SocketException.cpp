#include "SocketException.hpp"

using namespace std;

namespace MiniSocket {

// SocketException
SocketException::SocketException(const string &message) :
    runtime_error(message) 
{
}

SocketException::SocketException(const string &message, const string &detail) :
    runtime_error(message + ": " + detail) 
{
}

}   // namespace MiniSocket
