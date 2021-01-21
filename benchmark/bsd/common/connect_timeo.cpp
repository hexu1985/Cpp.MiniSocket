/* include connect_timeo */
#include "connect_timeo.hpp"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

// 指定超时时间的connect函数
// connect成功返回1，
// connect报错返回-1，
// connect超时返回0
int connect_timeo(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
{
    int ret = 0;
    int flags = 0;
    int saved_errno = 0;

    // 获取套接字flags
    if ((flags = fcntl(sockfd, F_GETFL, 0)) < 0) {
        return -1;
    }

    // 设置套接字为非阻塞的
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        return -1;
    }

    // 执行connect，
    // 因为套接字设为NONBLOCK，通常情况下，连接在connect()返回
    // 之前是不会建立的，因此它会返回EINPROGRESS错误，如果返回
    // 任何其他错误，则要进行错误处理
    ret = connect(sockfd, saptr, salen);
    if (ret == 0) {     // connect ok
        ret = 1;
        goto restore_flags;
    } 
    if (ret < 0 && errno != EINPROGRESS) {  // 其他错误
        saved_errno = errno;
        goto restore_flags;
    }

    // 设置超时时间
    struct timeval tv;
    tv.tv_sec = nsec;
    tv.tv_usec = 0;

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(sockfd, &fdset);

    ret = select(sockfd+1, NULL, &fdset, NULL, &tv);
    // ret < 0 错误处理
    // ret == 0 select超时
    // ret == 1 connect ok
    if (ret < 0) {  // 错误处理
        saved_errno = errno;
    } 

restore_flags:
    fcntl(sockfd, F_SETFL, flags);
    errno = saved_errno;
    return ret;
}

