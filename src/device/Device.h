// Copyright 2019 Johan Lasperas
#ifndef SRC_DEVICE_DEVICE_H_
#define SRC_DEVICE_DEVICE_H_

#include <cstdint>
#include <vector>
#include "rtmidi/RtMidi.h"
#include "util/Color.h"
#include "util/UString.h"

class Mapping;
class Device {
 public:
  Device() : m_mapping_idx(0) {}

  void addMapping(Mapping* mapping);

 protected:
  virtual void send(const ustring& message) = 0;

  void noteOn(int channel, int note);
  void noteOff(int channel, int note);
  void setColors(const std::vector<Color>& colors);
  void changeMapping();

  int m_mapping_idx;
  std::vector<Mapping*> m_mappings;
};

#endif  // SRC_DEVICE_DEVICE_H_
