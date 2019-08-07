#include "SYSException.hpp"

namespace MiniSocket {

using std::string;

SYSException::SYSException(const string &message, int error):
    SocketException(get_sys_error_str(message, error), make_sys_error(error))
{
}

SYSException::~SYSException()
{
}

void sys_error(const string &message, int error)
{
    throw SYSException(message, error);
}

void sys_error(const string &message)
{
    throw SYSException(message, get_last_sys_error());
}

}   // namespace MiniSocket
