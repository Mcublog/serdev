/**
 * @file USocketServDev.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-01-02
 *
 * @copyright Viacheslav mcublog (c) 2025
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "USocketServDev.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME     usocksrv
#define LOG_MODULE_LEVEL    (3)
#include <log_libs.h>
//<<----------------------

//>>---------------------- Local declaration
//<<----------------------

//>>---------------------- Local definitions
//<<----------------------

//>>---------------------- Exported function
USocketServerDevice::USocketServerDevice(const char *socketname, void *(*read_thread)(void *))
{
    m_socket_name = socketname;
    m_read_thread = read_thread;
}

/**
 * @brief
 *
 * @param ctl -- control
 * @return true
 * @return false
 */
bool USocketServerDevice::init(ios_ctl_t *ctl)
{
    Serial::init(ctl);

    LOG_INFO("Usage: %s", m_socket_name);
    m_server_stream = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_server_stream == -1)
    {
        LOG_ERROR("Error on socket() call");
        exit(1);
    }

    if (m_read_thread == nullptr)
    {
        LOG_ERROR("m_read_thread == nullptr");
        return false;
    }

    struct sockaddr_un local = {};
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, m_socket_name);
    unlink(local.sun_path);

    int err = bind(m_server_stream, (struct sockaddr *)&local, sizeof(local));
    if (err != 0)
    {
        LOG_ERROR("Error on binding socket");
        exit(1);
    }

    if (listen(m_server_stream, 1) != 0)
    {
        LOG_ERROR("Error on listen call");
        exit(1);
    }

    LOG_ERROR("Waiting for connection....");
    unsigned int sock_len = 0;
    struct sockaddr_un remote = {};
    m_client_stream = accept(m_client_stream, (struct sockaddr *)&remote, &sock_len);
    if (m_client_stream == -1)
    {
        LOG_ERROR("Error on accept() call");
        exit(1);
    }
    LOG_INFO("Server connected");

    pthread_create(&m_thread_id, NULL, m_read_thread, NULL);

    return true;
}

/**
 * @brief Write to file
 *
 * @param data
 * @param size
 * @return true
 * @return false
 */
bool USocketServerDevice::write(uint8_t *data, uint32_t size)
{
    LOG_DEBUG("Write: %d", size);
    if (m_client_stream == (-1))
    {
        LOG_ERROR("io stream");
        return false;
    }
    ssize_t writed_count = ::send(m_client_stream, data, size, 0);
    return writed_count == size;
}

/**
 * @brief Redifine irq handler
 *
 * @param irq
 */
void USocketServerDevice::register_irq(ios_irq_handler_t irq)
{
    m_irq_handler = irq;
}

uint8_t USocketServerDevice::read(bool *succsess)
{
    *succsess = false;
    if (m_client_stream == (-1))
    {
        LOG_ERROR("io stream");
        return 0;
    }
    uint8_t byte = 0;
    long size = ::recv(m_client_stream, &byte, 1, 0);
    *succsess = size > 0;
    return byte;
}
//<<----------------------