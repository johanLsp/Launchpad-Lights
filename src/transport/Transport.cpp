#include "transport/Transport.h"
#include "device/Device.h"

void Transport::receive(const ustring& message) {
  if (m_device) {
    m_device->receive(m_type, message);
  }
}
