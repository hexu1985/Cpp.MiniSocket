#ifndef DG_CLI_CONNECTED_INC
#define DG_CLI_CONNECTED_INC

#include <iostream>
#include "mini_socket.hpp"

void
dg_cli(std::istream &in, MiniSocket::UDPClientSocket &sock);

#endif
