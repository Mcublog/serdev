
#ifndef TTYDEVICE_H
#define TTYDEVICE_H

#include <pthread.h>

#include "../Serial.hpp"

class TtyDevice final: public Serial
{
  private:
    const char *m_portname = "/dev/ttyS11";
    void *(*m_read_thread)(void *) = nullptr;

    pthread_t m_thread_id;

    int m_io_stream;

  public:
    TtyDevice(const char *portname, void *(*read_thread)(void *));

    bool init(ios_ctl_t *ctl) override;
    bool init(const char *portname, ios_ctl_t *ctl) override;

    bool helth(){return true;};
    bool write(const uint8_t *data, uint32_t size) override;
    uint8_t read(bool *succsess) override;

  // Only emulation extend funtion
    void register_irq(ios_irq_handler_t irq);
};

#endif // TTYDEVICE_H