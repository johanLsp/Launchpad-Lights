// Copyright 2019 Johan Lasperas
#ifndef SRC_TRANSPORT_TRANSPORT_H_
#define SRC_TRANSPORT_TRANSPORT_H_

#include <functional>
#include <string>

#include "rtmidi/RtMidi.h"

class Device;

class Transport {
 public:
  enum Type {INVALID, MIDI, COLOR};
  Transport() : m_device(nullptr), m_connected(false) {}
  void setDevice(Device* device) { m_device = device; }

  virtual bool isConnected() { return m_connected; }

  virtual void send(const std::string& message) = 0;

 protected:
  virtual void receive(const std::string& message);
  virtual Type type() { return Type::INVALID; }

  bool m_connected;

 private:
  Device* m_device;
};

class Midi : public Transport {
  Type type() override { return Type::MIDI; }
};

#endif  // SRC_TRANSPORT_TRANSPORT_H_
