// Copyright 2019 Johan Lasperas
#include "device/Device.h"
#include "mapping/Mapping.h"


void Device::send(const ustring& message) {
  m_transport->send(message);
}

void Device::addMapping(Mapping* mapping) {
  m_mappings.push_back(mapping);
  if (m_mappings.size() == 1)
    m_mappings[0]->start();
}

void Device::noteOn(int channel, int note) {
  m_mappings[m_mapping_idx]->noteOn(channel, note);
}

void Device::noteOff(int channel, int note) {
  m_mappings[m_mapping_idx]->noteOff(channel, note);
}

void Device::setColors(const std::vector<Color>& colors) {
  m_mappings[m_mapping_idx]->setColors(colors);
}

void Device::changeMapping() {
  m_mappings[m_mapping_idx]->stop();
  m_mapping_idx++;
  if (m_mapping_idx >= m_mappings.size())
    m_mapping_idx = 0;
  m_mappings[m_mapping_idx]->start();
}
