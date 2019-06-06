/**
 * @file tcp_connect.hpp
 * @brief tcp connect接口
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-05-24
 *
 * @see UNIX Network Programming Volume 1: The Sockets Networking API, Third Edition \n
 * https://github.com/hexu1985/UNIX.Network.Programming
 */
#ifndef PNL_MINI_SOCKET_TCP_CONNECT_INC
#define PNL_MINI_SOCKET_TCP_CONNECT_INC

#include <memory>
#include "mini_socket.hpp"

namespace MiniSocket {

std::shared_ptr<TCPSocket> tcp_connect(const char *host, const char *serv);

}   // namespace MiniSocket

#endif
