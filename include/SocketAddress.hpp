/**
 * @file SocketAddress.hpp
 * @brief 封装Socket地址的类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-05
 */
#ifndef MINI_SOCKET_SOCKET_ADDRESS_INC
#define MINI_SOCKET_SOCKET_ADDRESS_INC

#include <string>
#include <tuple>
#include "SocketCommon.hpp"

namespace MiniSocket {

/**
 * @brief 封装Socket地址的类, 同时支持IPv4和IPv6
 */
class SocketAddress {
public:
    /**
     * @brief 构造一个空的SocketAddress
     */
    SocketAddress() = default; 

    /**
     * @brief 根据指定ip+port构造一个SocketAddress
     *
     * @param address ip地址的字符串
     * @param port 端口号
     *
     * @note 如果地址无效, 会抛出SocketException异常
     */
    SocketAddress(const char *address, uint16_t port); 

    /**
     * @brief 从sockaddr指针转换成一个SocketAddress
     *
     * @param addrVal 指向sockaddr的指针
     * @param addrLenVal 实际sockaddr的长度
     */
    SocketAddress(sockaddr *addrVal, socklen_t addrLenVal);

    /**
     * @brief 根据指定ip+port重新设置一个SocketAddress
     *
     * @param address ip地址的字符串
     * @param port 端口号
     *
     * @return 如果是有效地址, 返回true; 否则返回false
     */
    bool setAddressPort(const char *address, uint16_t port); 

    /**
     * @brief 将SocketAddress转换成可打印格式
     *
     * @return ipv4: xxx.xxx.xxx.xxx:port, ipv6: [xxx:xxx:...:xxx]:port
     */
    std::string toString() const;

    /**
     * @brief 获取可打印地址和port号
     *
     * @return 包含地址和端口号的元组
     */
    std::tuple<std::string, uint16_t> getAddressPort() const;

    /**
     * @brief 转换成sockaddr指针
     *
     * @return 指向sockaddr的指针
     */
    sockaddr *getSockaddr() const; 

    /**
     * @brief 获取sockaddr实际的长度
     *
     * @return sockaddr长度
     */
    socklen_t getSockaddrLen() const; 

    /**
     * @brief 获取sockaddr类型(网络层协议)
     *
     * @return NetworkLayerType枚举 
     */
    NetworkLayerType getNetworkLayerType() const;

private:
    sockaddr_storage addr_ = {};
    socklen_t addrLen_ = sizeof(addr_);
};

}   // MiniSocket

#endif
