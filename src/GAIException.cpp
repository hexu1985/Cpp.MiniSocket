#include "GAIException.hpp"

namespace mini_socket {

using std::string;

GAIException::GAIException(const string &message, int error):
    SocketException(get_gai_error_str(message, error), make_gai_error(error))
{
}

GAIException::~GAIException()
{
}

void gai_error(const string &message, int error)
{
#if defined (WIN32) || defined (_WIN32)
    throw GAIException(message, get_last_sys_error());
#else
    throw GAIException(message, error);
#endif
}

}   // namespace mini_socket
