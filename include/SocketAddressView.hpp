/**
 * @file SocketAddressView.hpp
 * @brief 封装Socket地址的引用的类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-05
 */
#ifndef MINI_SOCKET_SOCKET_ADDRESS_VIEW_INC
#define MINI_SOCKET_SOCKET_ADDRESS_VIEW_INC

#include <string>
#include <tuple>
#include "SocketCommon.hpp"

namespace mini_socket {

/**
 * @brief 封装Socket地址的引用的类, 同时支持IPv4和IPv6
 * @note 该类并不管理指向的sockaddr的内存
 */
class SocketAddressView {
public:
    /**
     * @brief 从sockaddr指针创建一个SocketAddressView
     *
     * @param addrVal 指向sockaddr的指针
     * @param addrLenVal 实际sockaddr的长度
     */
    SocketAddressView(const sockaddr *addrVal = NULL, socklen_t addrLenVal = 0);

    /**
     * @brief 将SocketAddressView转换成可打印格式
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
    const sockaddr *getSockaddr() const; 

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
    const sockaddr *addr_ = 0;
    socklen_t addrLen_ = 0;
};

}   // mini_socket

#endif
