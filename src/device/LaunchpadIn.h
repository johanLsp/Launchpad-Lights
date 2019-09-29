// Copyright 2019 Johan Lasperas
#ifndef SRC_DEVICE_LAUNCHPADIN_H_
#define SRC_DEVICE_LAUNCHPADIN_H_

#include <cstdint>
#include <vector>
#include "rtmidi/RtMidi.h"

#include "device/DeviceIn.h"
#include "mapping/Mapping.h"

class LaunchpadIn : public DeviceIn {
 public:
  LaunchpadIn();
  ~LaunchpadIn();

  void receive(std::vector<unsigned char> *message);
  bool isConnected() { return m_connected; }

 private:
  RtMidiIn *m_input;
  bool m_connected;
};

#endif  // SRC_DEVICE_LAUNCHPADIN_H_
