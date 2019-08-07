/**
 * @file mini_socket.hpp
 * @brief 一个简单的跨平台的C++网络编程库
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-05-22
 *
 * @see TCP/IP Sockets in C Second Edition, Chapter 8, PracticalSocket lib \n
 * https://github.com/hexu1985/TCP.IP.Sockets.in.C
 */
#ifndef MINI_SOCKET_INC
#define MINI_SOCKET_INC

#include "SocketException.hpp"
#include "SYSException.hpp"
#include "GAIException.hpp"
#include "SocketError.hpp"
#include "SocketCommon.hpp"
#include "SocketAddress.hpp"
#include "SocketAddressView.hpp"
#include "Socket.hpp"
#include "CommunicatingSocket.hpp"
#include "TCPSocket.hpp"
#include "TCPServerSocket.hpp"
#include "UDPSocket.hpp"
#include "UDPClientSocket.hpp"
#include "DNSResolver.hpp"
#include "tcp_connect.hpp"
#include "udp_connect.hpp"

#endif
