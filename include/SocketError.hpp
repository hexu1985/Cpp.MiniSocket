/**
 * @file SocketError.hpp
 * @brief 一种简单的统一错误码: 包含错误类型和错误值
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-08-07
 */
#ifndef MINI_SOCKET_SOCKET_ERROR_INC
#define MINI_SOCKET_SOCKET_ERROR_INC

#include <string>

namespace mini_socket {

/**
 * @brief 一种简单的统一错误码: 包含错误类型和错误值
 */
struct SocketError {
    /**
     * @brief 错误类型
     */
    enum ErrorType {
        no_error = 0,             /**< 没有错误 */
        sys_error = 1,            /**< 系统层: use errno */
        gai_error = 2,            /**< getaddrinfo接口返回的错误码: EAI_* */
        usr_error = UINT16_MAX,   /**< 用户侧错误 */   
    };

    int type = no_error;     // 错误类型
    int code = 0;           // 错误值

    /**
     * @brief 创建一个错误码对象
     *
     * @param type_ 错误类型
     * @param code_ 错误值
     */
    SocketError(int type_, int code_): type(type_), code(code_) {}

    /**
     * @brief 创建一个默认错误码对象
     */
    SocketError() = default;
};

/**
 * @brief 创建系统类型的错误码
 *
 * @param error 错误值(一般为系统调用返回的错误码, 或errno值)
 *
 * @return 系统类型的错误码
 */
inline
SocketError make_sys_error(int error)
{
    return SocketError(SocketError::sys_error, error);
}

/**
 * @brief 创建获取地址信息类型的错误码
 *
 * @param error 错误值(一般为getaddrinfo返回的错误码)
 *
 * @return 获取地址信息类型的错误码
 */
inline
SocketError make_gai_error(int error)
{
    return SocketError(SocketError::gai_error, error);
}

/**
 * @brief 获取最近一次的系统错误码
 *
 * @return 系统错误码
 */
int get_last_sys_error();

inline
void get_last_sys_error(SocketError &ec)
{
    ec = make_sys_error(get_last_sys_error());
}

std::string get_sys_error_str(int error);
std::string get_gai_error_str(int error);
std::string get_sys_error_str(const std::string &title, int error);
std::string get_gai_error_str(const std::string &title, int error);

}   // mini_socket

#endif
