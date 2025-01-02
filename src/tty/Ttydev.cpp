/**
 * @file Ttydev.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief TTY Serial device implemenation
 * @version 0.1
 * @date 2023-02-26
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "Ttydev.hpp"
#include "../io/tty.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME     ttydev
#define LOG_MODULE_LEVEL    (3)
#include <log_libs.h>
//<<----------------------

//>>---------------------- Local declaration
//<<----------------------

//>>---------------------- Local definitions
//<<----------------------

//>>---------------------- Exported function
TtyDevice::TtyDevice(const char *portname, void*(*read_thread)(void*))
{
    m_portname = portname;
    m_read_thread = read_thread;
}

/**
 * @brief
 *
 * @param ctl -- control
 * @return true
 * @return false
 */
bool TtyDevice::init(ios_ctl_t *ctl)
{
    Serial::init(ctl);

    LOG_INFO("Usage: %s", m_portname);
    m_io_stream = open(m_portname, O_RDWR | O_NONBLOCK);
    if (m_io_stream == (-1))
    {
        perror("open");
        exit(1);
    }

    const std::string portname{m_portname};
    if ((portname.find(std::string("ACM")) != std::string::npos) ||
        (portname.find(std::string("USB")) != std::string::npos))
    {
        LOG_INFO("%s is hardware, init it", m_portname);
        set_interface_attribs(m_io_stream, B115200);
    }

    if (m_read_thread == nullptr)
    {
        LOG_ERROR("m_read_thread == nullptr");
        return false;
    }
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
bool TtyDevice::write(uint8_t *data, uint32_t size)
{
    LOG_DEBUG("Write: %d", size);
    if (m_io_stream == (-1))
    {
        LOG_ERROR("io stream");
        return false;
    }
    ssize_t writed_count = ::write(m_io_stream, data, size);
    return writed_count == size;
}

/**
 * @brief Redifine irq handler
 *
 * @param irq
 */
void TtyDevice::register_irq(ios_irq_handler_t irq)
{
    m_irq_handler = irq;
}

uint8_t TtyDevice::read(bool *succsess)
{
    uint8_t byte = 0;
    long size = ::read(m_io_stream, &byte, 1);
    *succsess = size > 0;
    return byte;
}
//<<----------------------