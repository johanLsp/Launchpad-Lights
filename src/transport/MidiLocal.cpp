// Copyright 2019 Johan Lasperas
#include "transport/MidiLocal.h"

#include <string>

void receive(double timestamp, std::vector<unsigned char>* message,
            void* data) {
  MidiLocal* midi = reinterpret_cast<MidiLocal*>(data);
  ustring message_str(message->begin(), message->end());
  midi->receiveCallback(message_str);
}

MidiLocal::MidiLocal() {
  m_output = new RtMidiOut();
  unsigned int nPorts = m_output->getPortCount();

  bool connected = false;
  for (int i = 0; i < nPorts; i++) {
    std::string name = m_output->getPortName(i);
    if (name.compare(0, 9, "Launchpad") == 0) {
      std::cout << "Output : " << name << std::endl;
      m_output->openPort(i);
      connected = true;
    }
  }
  if (!connected) return;
  m_input = new RtMidiIn();
  // Check available ports.
  nPorts = m_input->getPortCount();
  for (int i = 0; i < nPorts; i++) {
    std::string name = m_input->getPortName(i);
    if (name.compare(0, 9, "Launchpad") == 0) {
      m_input->openPort(i);
      m_input->setCallback(&::receive, this);
        connected = true;
    }
  }
  m_connected = connected;
}

MidiLocal::~MidiLocal() {
  delete m_output;
  delete m_input;
}

void MidiLocal::send(const ustring& message) {
  std::vector<unsigned char> buffer(message.begin(), message.end());
  m_output->sendMessage(&buffer);
}
