// Copyright 2019 Johan Lasperas
#include "transport/MidiLocal.h"

#include <string>

void receive(double timestamp, std::vector<unsigned char>* message,
            void* data) {
  MidiLocal* midi = reinterpret_cast<MidiLocal*>(data);
  std::string message_str(message->begin(), message->end());
  midi->receiveCallback(message_str);
}

MidiLocal::MidiLocal() {
  m_output = new RtMidiOut();
  m_input = new RtMidiIn();
  m_input->setCallback(&::receive, this);
  connect();
}

MidiLocal::~MidiLocal() {
  delete m_output;
  delete m_input;
}

bool MidiLocal::isConnected() {
  return connect();
}

bool MidiLocal::connect() {
  int idx_in = findLaunchpad(m_input);
  int idx_out = findLaunchpad(m_output);

  if (idx_in != -1 && idx_out != -1) {
    if (m_connected) return true;
    m_input->openPort(idx_in);
    m_output->openPort(idx_out);
    m_connected = true;
    return true;
  }

  if (!m_connected) return false;

  m_input->closePort();
  m_output->closePort();
  m_connected = false;
  return false;
}

int MidiLocal::findLaunchpad(RtMidi* midi) {
  for (int i = 0; i < midi->getPortCount(); i++) {
    std::string name = midi->getPortName(i);
    if (name.compare(0, 9, "Launchpad") == 0) {
      return i;
    }
  }
  return -1;
}

void MidiLocal::send(const std::string& message) {
  std::vector<unsigned char> buffer(message.begin(), message.end());
  m_output->sendMessage(&buffer);
}
