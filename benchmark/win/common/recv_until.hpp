#ifndef UNP_RECV_UNTIL_INC
#define UNP_RECV_UNTIL_INC

#include <winsock2.h>

/**
 * @brief 从指定sockfd接收数据, 指定遇到指定结束符或者读满缓存区
 *
 * @param sockfd 接收数据的套接字描述符
 * @param buffer 接收数据缓存区
 * @param maxlen 接收数据缓存区最大长度
 * @param term 结束符
 *
 * @return 如果成功返回读取字节数(>= 0, 0表示EOF-对端close), 否则返回-1
 */
int recv_until(SOCKET sockfd, void *buffer, int maxlen, char term);

#endif
