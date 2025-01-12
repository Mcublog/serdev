
#ifndef SERDEV_UTILS_SOCKETS_H
#define SERDEV_UTILS_SOCKETS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    bool is_unix_addr(const char *portname);
    int socket_bind(int fd, const char *portname, unsigned short family);
    int socket_connect(int fd, const char *portname, unsigned short family);
    int socket_accept(int fd, unsigned short family);

#ifdef __cplusplus
}
#endif

#endif // SERDEV_UTILS_SOCKETS_H