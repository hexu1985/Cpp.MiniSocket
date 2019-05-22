#ifndef DG_CLI_INC
#define DG_CLI_INC

#include <iostream>
#include "mini_socket.hpp"

void
dg_cli(std::istream &in, MiniSocket::UDPSocket &sock, MiniSocket::SocketAddress &addr);

#endif
