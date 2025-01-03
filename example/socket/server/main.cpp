/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-01-02
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "common.h"
#include "src/Serial.hpp"
#include "src/usocket/USocketServDev.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME serv
#define LOG_MODULE_LEVEL (3)
#include <log_libs.h>
//<<----------------------

//>>---------------------- Local declaration
static void *reading(void*);
static USocketServerDevice m_serv = USocketServerDevice("/tmp/usocket", reading);

static constexpr uint32_t kSDeviceBufferSize = 256;
static uint8_t m_input_buffer[kSDeviceBufferSize];
static uint8_t m_output_buffer[kSDeviceBufferSize];

std::atomic_bool m_get_msg = false;
//<<----------------------

//>>---------------------- Local definitions
static void *reading(void*)
{
    serial_read_process(&m_serv);
    return nullptr;
}

static uint32_t rx_irq_handler(ios_chunk_t *chunk)
{
    LOG_INFO("rx[%002d]", chunk->size);
    if (chunk->data[0] = '\n')
        m_get_msg = true;
    return 0;
}
//<<----------------------

int main(int argc, char *argv[])
{
    LOG_INFO("Server run");
    ios_ctl_t comm_ctl = {{m_input_buffer, kSDeviceBufferSize},
                          {m_output_buffer, kSDeviceBufferSize},
                          rx_irq_handler};
    m_serv.init(&comm_ctl);
    while(1)
    {
        if (m_get_msg)
        {
            m_get_msg = false;
            m_serv.write((uint8_t*)"Hello\n", strlen("Hello\n"));
        }
        delay_ms(1);
    }
    return 0;
}