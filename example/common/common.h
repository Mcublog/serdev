
#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

#include "src/Serial.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

    void delay_ms(uint32_t ms);
    void serial_read_process(Serial *ser);

#ifdef __cplusplus
}
#endif

#endif // COMMON_H