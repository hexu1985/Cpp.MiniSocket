#ifndef UNP_RECV_ALL_INC
#define UNP_RECV_ALL_INC

#include <sys/types.h>

/**
 * @brief 从sockfd接收指定长度的数据
 *
 * @param sockfd 接收数据的套接字描述符
 * @param buffer 接收数据的缓存区
 * @param buf_len 接收数据的长度
 *
 * @return 如果成功返回buf_len, 否则返回-1
 */
ssize_t	recv_all(int sockfd, void *buffer, size_t buf_len);

#endif

