
#ifndef APP_INTERFACES_SERIAL_SERIAL_HPP
#define APP_INTERFACES_SERIAL_SERIAL_HPP

#include "io/types.h"

class Serial
{
  protected:
    ios_irq_handler_t m_irq_handler = nullptr;
    const char *m_portname = nullptr;

  public:

    virtual bool init(ios_ctl_t *ctl)
    {
        m_irq_handler = ctl->irq_handler;
        return true;
    };

    virtual bool init(const char *portname, ios_ctl_t *ctl) = 0;

    virtual bool helth() = 0;
    virtual bool write(const uint8_t *data, uint32_t size) = 0;
    virtual uint8_t read(bool *succsess) = 0;

    uint32_t irq_handler(ios_chunk_t *chunk);

    virtual void register_irq(ios_irq_handler_t irq) = 0;
};

#endif // APP_INTERFACES_SERIAL_SERIAL_HPP