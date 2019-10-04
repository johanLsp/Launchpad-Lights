// Copyright 2019 Johan Lasperas
#ifndef SRC_DEVICE_DEVICE_H_
#define SRC_DEVICE_DEVICE_H_

#include <cstdint>
#include <vector>
#include "rtmidi/RtMidi.h"
#include "transport/Transport.h"
#include "util/Color.h"
#include "util/UString.h"

class Mapping;
class Device {
 public:
  explicit Device(Transport* transport)
      : m_transport(transport), m_mapping_idx(0) {
    m_transport->setDevice(this);
  }

  virtual void receive(Transport::Type type, const ustring& message) = 0;

  void addMapping(Mapping* mapping);
  bool isConnected() { return m_transport->isConnected(); }

 protected:
  void send(const ustring& message);
  void noteOn(int channel, int note);
  void noteOff(int channel, int note);
  void setColors(const std::vector<Color>& colors);
  void changeMapping();

  int m_mapping_idx;
  std::vector<Mapping*> m_mappings;
  Transport* m_transport;
};

#endif  // SRC_DEVICE_DEVICE_H_
