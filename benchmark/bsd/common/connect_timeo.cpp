/* include connect_timeo */
#include "connect_timeo.hpp"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

int
connect_timeo(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
{
    int ret = 0;
    int flags = 0;

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
        goto restore_flags;
    } 
    if (ret < 0 && errno != EINPROGRESS) {  // 其他错误
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
    if (ret < 0) {  // 错误处理
        goto restore_flags;
    } else if (ret == 0) {  // select超时
        ret = -1;
        errno = ETIMEDOUT;
        goto restore_flags;
    } else { // ret == 1 connect ok
        ret = 0;
    }

restore_flags:
    fcntl(sockfd, F_SETFL, flags);
    return ret;
}

