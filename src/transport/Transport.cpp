#include "transport/Transport.h"
#include "device/Device.h"

void Transport::receive(const ustring& message) {
  if (m_device) {
    m_connected = true;
    m_device->receive(type(), message);
  }
}
