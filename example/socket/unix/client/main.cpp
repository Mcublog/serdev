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
#include "src/usocket/USocketClientDev.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME clnt
#define LOG_MODULE_LEVEL (3)
#include <log_libs.h>
//<<----------------------

//>>---------------------- Local declaration
static void *reading(void*);
static USocketClientDevice m_client = USocketClientDevice("/tmp/usocket", reading);

static constexpr uint32_t kSDeviceBufferSize = 256;
static uint8_t m_input_buffer[kSDeviceBufferSize];
static uint8_t m_output_buffer[kSDeviceBufferSize];

std::atomic_bool m_get_msg = false;

static constexpr uint8_t kPing[] = "Ping\n";
static constexpr uint32_t kPingSize = sizeof(kPing) - 1;
//<<----------------------

//>>---------------------- Local definitions
static void *reading(void*)
{
    serial_read_process(&m_client);
    return nullptr;
}

static uint32_t rx_irq_handler(ios_chunk_t *chunk)
{
    LOG_DEBUG("rx[%002d]", chunk->size);
    if (chunk->data[0] = '\n')
        m_get_msg = true;
    return 0;
}
//<<----------------------

int main(int argc, char *argv[])
{
    LOG_INFO("Client run");
    ios_ctl_t comm_ctl = {{m_input_buffer, kSDeviceBufferSize},
                          {m_output_buffer, kSDeviceBufferSize},
                          rx_irq_handler};
    m_client.init(&comm_ctl);
    m_client.write(kPing, kPingSize);
    while(1)
    {
        if (m_get_msg)
        {
            LOG_INFO("rx pong, send ping");
            m_get_msg = false;
            m_client.write(kPing, kPingSize);
        }
        delay_ms(1000);
    }
    return 0;
}