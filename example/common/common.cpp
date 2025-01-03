/**
 * @file common.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-01-03
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */
#include <cstdlib>
#include <unistd.h>

#include "common.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME cmn
#define LOG_MODULE_LEVEL (3)
#include <log_libs.h>
//<<----------------------

void delay_ms(uint32_t ms)
{
    static constexpr uint32_t kMsMult = 1000;
    usleep(ms * kMsMult);
}

void serial_read_process(Serial *ser)
{
    while (1)
    {
        bool succsess = false;
        uint8_t byte = ser->read(&succsess);
        if (succsess == false)
        {
            if (ser->helth() == false)
            {
                LOG_ERROR("connection lost");
                exit(1);
            }

            delay_ms(1);
            continue;
        }
        ios_chunk_t data = {&byte, 1};
        ser->irq_handler(&data);
    }
}