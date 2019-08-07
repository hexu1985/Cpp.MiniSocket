#ifndef MINI_SOCKET_SOCKET_ERROR_INC
#define MINI_SOCKET_SOCKET_ERROR_INC

namespace MiniSocket {

/**
 * @brief 错误类型
 */
enum class ErrorType: uint32_t {
    UNKNOWN = 0,        /**< 未知类型 */
    SYS = 1,            /**< 系统层: use errno */
    GAI = 2,            /**< getaddrinfo接口返回的错误码: EAI_* */
    USR = UINT16_MAX,   /**< 用户侧错误 */
};

/**
 * @brief 一种简单的统一错误码: 包含错误类型和错误值
 */
struct ErrorCode {
    int type = (int) ErrorType::UNKNOWN;    // 错误类型
    int value = 0;                          // 错误值

    /**
     * @brief 创建一个错误码对象
     *
     * @param type_ 错误类型
     * @param value_ 错误值
     */
    ErrorCode(int type_, int value_): type(type_), value(value_) {}

    /**
     * @brief 创建一个默认错误码对象
     */
    ErrorCode() = default;
};

/**
 * @brief 创建系统类型的错误码
 *
 * @param error 错误值(一般为系统调用返回的错误码, 或errno值)
 *
 * @return 系统类型的错误码
 */
inline
ErrorCode make_sys_error(int error)
{
    return ErrorCode((int) ErrorType::SYS, error);
}

/**
 * @brief 创建获取地址信息类型的错误码
 *
 * @param error 错误值(一般为getaddrinfo返回的错误码)
 *
 * @return 获取地址信息类型的错误码
 */
inline
ErrorCode make_gai_error(int error)
{
    return ErrorCode((int) ErrorType::GAI, error);
}

/**
 * @brief 获取最近一次的系统错误码
 *
 * @return 系统错误码
 */
ErrorCode get_last_sys_error();

}   // MiniSocket

#endif
