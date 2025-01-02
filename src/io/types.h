
#ifndef APP_IO_SERIAL_TYPES_H
#define APP_IO_SERIAL_TYPES_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint8_t *data;
    uint32_t limit;
} ios_buf_t;

typedef struct
{
    uint8_t *data;
    uint32_t size;
} ios_chunk_t;

typedef uint32_t (*ios_irq_handler_t)(ios_chunk_t *chunk);

typedef struct
{
    ios_buf_t in;
    ios_buf_t out;
    ios_irq_handler_t irq_handler;
} ios_ctl_t;

#endif // APP_IO_SERIAL_TYPES_H