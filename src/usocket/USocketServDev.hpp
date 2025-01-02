
#ifndef SOCKETDEVICE_H
#define SOCKETDEVICE_H

#include <pthread.h>

#include "../Serial.hpp"

class USocketServerDevice final: public Serial
{
  private:
    const char *m_socket_name = "socket_name";
    void *(*m_read_thread)(void *) = nullptr;

    pthread_t m_thread_id;
    int m_server_stream = -1;
    int m_client_stream = -1;

  public:
    USocketServerDevice(const char *socketname, void *(*read_thread)(void *));

    bool init(ios_ctl_t *ctl);
    bool helth(){return true;};
    bool write(uint8_t *data, uint32_t size);
    uint8_t read(bool *succsess);

  // Only emulation extend funtion
    void register_irq(ios_irq_handler_t irq);
};

#endif // SOCKETDEVICE_H