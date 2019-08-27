/**
 * @file SocketCommon.hpp
 * @brief 一些socket通用定义和函数
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-07
 */
#ifndef MINI_SOCKET_SOCKET_COMMON_INC
#define MINI_SOCKET_SOCKET_COMMON_INC

#if defined WIN32 or defined _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include <string>
#include <tuple>

namespace mini_socket {

/// IP版本号
enum class NetworkLayerType {
    UNKNOWN = UINT16_MAX,   /**< 未知协议 */
    IPv4 = AF_INET,         /**< IPv4协议 */
    IPv6 = AF_INET6,        /**< IPv6协议 */
};

/// 传输层协议类型
enum class TransportLayerType {
    UNKNOWN = UINT16_MAX,   /**< 未知协议 */
    TCP = SOCK_STREAM,      /**< TCP协议 */
    UDP = SOCK_DGRAM,       /**< UDP协议 */
};

/**
 * @brief 将sockaddr地址转换成ip+port的tuple
 *
 * @param sa sockaddr地址的指针
 * @param salen sockaddr地址的长度
 *
 * @return ip+port的tuple
 */
std::tuple<std::string, uint16_t> get_address_port(const sockaddr *sa, socklen_t salen);

/**
 * @brief 将sockaddr地址转换成string类型
 *
 * @param sa sockaddr地址的指针
 * @param salen sockaddr地址的长度
 *
 * @return ipv4: xxx.xxx.xxx.xxx:port, ipv6: [xxx:xxx:...:xxx]:port
 */
std::string to_string(const sockaddr *sa, socklen_t salen);

/**
 * @brief 获取sockaddr地址的IP版本号
 *
 * @param sa sockaddr地址的指针
 * @param salen sockaddr地址的长度
 *
 * @return IP版本号
 */
mini_socket::NetworkLayerType get_network_layer_type(const sockaddr *sa, socklen_t salen);

}   // mini_socket

#endif
