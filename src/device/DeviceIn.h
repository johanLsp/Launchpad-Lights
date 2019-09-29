// Copyright 2019 Johan Lasperas
#ifndef SRC_DEVICE_DEVICEIN_H_
#define SRC_DEVICE_DEVICEIN_H_

#include <cstdint>
#include <vector>
#include "rtmidi/RtMidi.h"
#include "mapping/Mapping.h"

class DeviceIn {
 public:
  DeviceIn() : m_mapping_idx(0) {}

  void addMapping(Mapping* mapping) {
      m_mappings.push_back(mapping);
      if (m_mappings.size() == 1)
          m_mappings[0]->start();
  }

 protected:
  void changeMapping() {
      m_mappings[m_mapping_idx]->stop();
      m_mapping_idx++;
      if (m_mapping_idx >= m_mappings.size())
          m_mapping_idx = 0;
      m_mappings[m_mapping_idx]->start();
  }

  int m_mapping_idx;
  std::vector<Mapping*> m_mappings;
};

#endif  // SRC_DEVICE_DEVICEIN_H_
