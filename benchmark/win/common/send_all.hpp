#ifndef UNP_SEND_ALL_INC
#define UNP_SEND_ALL_INC

#include <winsock2.h>

/**
 * @brief 向sockfd发送指定长度的数据
 *
 * @param sockfd 发送数据的套接字描述符
 * @param buffer 发送数据的内容
 * @param buf_len 发送数据的长度
 *
 * @return 如果成功返回buf_len, 否则返回-1
 */
int send_all(SOCKET sockfd, const void *buffer, int buf_len);

#endif
