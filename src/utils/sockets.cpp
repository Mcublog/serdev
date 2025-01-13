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
#include <arpa/inet.h>
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

//>>---------------------- Private
static int parse_inet_addr(const char* portname, in_addr_t *addr, int *port)
{
    char address[256] = {};
    if (sscanf(portname, "%255[^:]:%d", address, port) != 2)
    {
        LOG_ERROR("can't parse %s to address:port", portname);
        return -1;
    }
    return inet_pton(AF_INET, address, addr);
}

static int unix_socket_bind(int fd, const char *portname)
{
    struct sockaddr_un local = {};

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, portname);
    unlink(local.sun_path);

    return bind(fd, (struct sockaddr *)&local, sizeof(local));
}

static int unix_socket_connect(int fd, const char *portname)
{
    struct sockaddr_un remote = {};

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, portname);

    return connect(fd, (struct sockaddr *)&remote, sizeof(remote));
}

static int unix_socket_accept(int fd)
{
    unsigned int sock_len = 0;
    struct sockaddr_un remote = {};

    return accept(fd, (struct sockaddr *)&remote, &sock_len);
}

/**
 * @brief
 *
 * @param fd
 * @param portname
 * @return int
 */
static int inet_socket_bind(int fd, const char *portname)
{
    int port;
    in_addr_t addres;

    if (parse_inet_addr(portname, &addres, &port) == -1)
    {
        LOG_ERROR("can't parse %s to address:port", portname);
        return -1;
    }

    struct sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = addres;
    address.sin_port = htons(port);

    return bind(fd, (struct sockaddr *)&address, sizeof(address));
}

static int inet_socket_connect(int fd, const char *portname)
{
    int port;
    in_addr_t addres;

    if (parse_inet_addr(portname, &addres, &port) == -1)
    {
        LOG_ERROR("can't parse %s to address:port", portname);
        return -1;
    }

    struct sockaddr_in serv_addr = {};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = addres;
    serv_addr.sin_port = htons(port);

    return connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
}

static int inet_socket_accept(int fd)
{
    unsigned int sock_len = 0;
    struct sockaddr remote = {};

    return accept(fd, &remote, &sock_len);
}
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

int socket_open(const char *portname, unsigned short *family)
{
    int port;
    in_addr_t addres;

    *family = AF_INET;
    if (parse_inet_addr(portname, &addres, &port) == -1)
    {
        LOG_DEBUG("using unix family");
        *family = AF_UNIX;
    }
    return socket(*family, SOCK_STREAM, 0);
}

/**
 * @brief
 *
 * @param famaly
 * @param portname
 * @param fd
 * @return int
 */
int socket_bind(int fd, const char *portname, unsigned short family)
{
    if (family == AF_UNIX)
        return unix_socket_bind(fd, portname);
    return inet_socket_bind(fd, portname);
}

/**
 * @brief
 *
 * @param fd
 * @param portname
 * @param famaly
 * @return int
 */
int socket_connect(int fd, const char *portname, unsigned short family)
{
    if (family == AF_UNIX)
        return unix_socket_connect(fd, portname);
    return inet_socket_connect(fd, portname);
}

/**
 * @brief
 *
 * @param fd
 * @return int
 */
int socket_accept(int fd, unsigned short family)
{
    if (family == AF_UNIX)
        return unix_socket_accept(fd);
    return inet_socket_accept(fd);
}

/**
 * @brief Is 'localhost' in string portname
 *
 * @param portname
 * @return true
 * @return false
 */
bool is_localhost_address_string(const char *portname)
{
    return (std::string_view{portname}.find("localhost") != std::string_view::npos);
}
