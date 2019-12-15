#include "transport/Transport.h"
#include "device/Device.h"

void Transport::receive(const std::string& message) {
  if (m_device) {
    m_device->receive(type(), message);
  }
}
