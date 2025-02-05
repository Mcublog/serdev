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
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "../utils/sockets.h"
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
    m_portname = socketname;
    m_read_thread = read_thread;
}

bool USocketServerDevice::init(const char *portname, ios_ctl_t *ctl)
{
    m_portname = portname;
    return init(ctl);
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

    unsigned short family = 0;
    m_server_stream = socket_open(m_portname, &family);

    LOG_INFO("Usage: %s with family: %d", m_portname, family);
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

    int err = socket_bind(m_server_stream, m_portname, family);
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

    m_client_stream = socket_accept(m_server_stream, family);
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
bool USocketServerDevice::write(const uint8_t *data, uint32_t size)
{
    LOG_DEBUG("Write: %d", size);
    if (m_client_stream == (-1))
    {
        LOG_ERROR("connection lost");
        exit(1);
    }
    ssize_t writed_count = ::write(m_client_stream, data, size);
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
        LOG_ERROR("connection lost");
        exit(1);
    }
    uint8_t byte = 0;
    long size = ::read(m_client_stream, &byte, 1);
    *succsess = size > 0;
    return byte;
}

/**
 * @brief If we are here, the connection is lost
 *
 * @return true
 * @return false
 */
bool USocketServerDevice::helth()
{
    close(m_client_stream);
    close(m_server_stream);
    return false;
}
//<<----------------------