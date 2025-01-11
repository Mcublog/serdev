/**
 * @file sockets.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-01-11
 *
 * @copyright Viacheslav mcublog (c) 2025
 *
 */
#include <string_view>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "sockets.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME utlsock
#define LOG_MODULE_LEVEL (3)
#include <log_libs.h>
//<<----------------------

/**
 * @brief
 *
 * @param portname
 * @return true
 * @return false
 */
bool is_unix_addr(const char *portname)
{
    std::string_view name{portname};
    if (name.find(":") != std::string_view::npos)
        return false;
    return true;
}

/**
 * @brief
 *
 * @param famaly
 * @param portname
 * @param fd
 * @return int
 */
int socket_bind(int fd, const char *portname, unsigned short famaly)
{
    struct sockaddr_un local = {};

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, portname);
    unlink(local.sun_path);

    return bind(fd, (struct sockaddr *)&local, sizeof(local));
}

/**
 * @brief
 *
 * @param fd
 * @param portname
 * @param famaly
 * @return int
 */
int socket_connect(int fd, const char *portname, unsigned short famaly)
{
    struct sockaddr_un remote = {};

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, portname);

    return connect(fd, (struct sockaddr *)&remote, sizeof(remote));
}
