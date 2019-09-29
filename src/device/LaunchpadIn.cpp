// Copyright 2019 Johan Lasperas
#include "device/LaunchpadIn.h"

#include <string>

void recv_bind(double timestamp, std::vector<unsigned char>* message,
               void* userData) {
  LaunchpadIn* launchpad = reinterpret_cast<LaunchpadIn*>(userData);
  launchpad->receive(message);
}

LaunchpadIn::LaunchpadIn()
    : m_connected(false) {
  m_input = new RtMidiIn();

  // Check available ports.
  unsigned int nPorts = m_input->getPortCount();

  for (int i = 0; i < nPorts; i++) {
    std::string name = m_input->getPortName(i);
    if (name.compare(0, 9, "Launchpad") == 0) {
      m_connected = true;
      m_input->openPort(i);
      m_input->setCallback(&recv_bind, this);
    }
  }
}

LaunchpadIn::~LaunchpadIn() {
  delete m_input;
}

void LaunchpadIn::receive(std::vector<unsigned char>* message) {
  unsigned int nBytes = message->size();
  int channel = message->at(0);
  int note    = message->at(1);
  int command = message->at(2);

  switch (command) {
    case 127:
      if (note == 104) {
          changeMapping();
      } else {
          m_mappings[m_mapping_idx]->noteOn(channel, note);
      }
      break;
    case 0:
      m_mappings[m_mapping_idx]->noteOff(channel, note);
      break;
  }
}
