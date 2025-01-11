/**
 * @file USocketClientDev.cpp
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
#include "USocketClientDev.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME     usockclt
#define LOG_MODULE_LEVEL    (3)
#include <log_libs.h>
//<<----------------------

//>>---------------------- Local declaration
//<<----------------------

//>>---------------------- Local definitions
//<<----------------------

//>>---------------------- Exported function
USocketClientDevice::USocketClientDevice(const char *socketname, void *(*read_thread)(void *))
{
    m_portname = socketname;
    m_read_thread = read_thread;
}

/**
 * @brief
 *
 * @param portname
 * @param ctl
 * @return true
 * @return false
 */
bool USocketClientDevice::init(const char *portname, ios_ctl_t *ctl)
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
bool USocketClientDevice::init(ios_ctl_t *ctl)
{
    Serial::init(ctl);

    LOG_INFO("Usage: %s", m_portname);
    m_client_stream = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_client_stream == -1)
    {
        LOG_ERROR("Error on socket() call");
        exit(1);
    }

    if (m_read_thread == nullptr)
    {
        LOG_ERROR("m_read_thread == nullptr");
        exit(1);
    }

    LOG_INFO("Client: Trying to connect...");

    if (socket_connect(m_client_stream, m_portname, AF_UNIX) == -1)
    {
        LOG_ERROR("Client: Error on connect call");
        exit(1);
    }

    LOG_INFO("Client: Connected");

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
bool USocketClientDevice::write(const uint8_t *data, uint32_t size)
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
void USocketClientDevice::register_irq(ios_irq_handler_t irq)
{
    m_irq_handler = irq;
}

uint8_t USocketClientDevice::read(bool *succsess)
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
bool USocketClientDevice::helth()
{
    close(m_client_stream);
    return false;
}
//<<----------------------