// Copyright 2019 Johan Lasperas
#include "device/Launchpad.h"

#include <functional>

namespace {
  // See Launchpad MKII Programmer's Reference Manual.
  static const ustring kNoteOff = {128_u};
  static const ustring kNoteOn = {144_u};
  static const ustring kFlash = {145_u};
  static const ustring kPulse = {146_u};
  static const ustring kCCFlash = {177_u};
  static const ustring kCCPulse = {178_u};
  static const ustring kSysEx = {240_u, 0_u, 32_u, 41_u, 2_u, 24_u};
  static const ustring kSysExEnd = {247_u};
  static const ustring kSysExLedStd = kSysEx + 10_u;
  static const ustring kSysExLedRGB = kSysEx + 11_u;
  static const ustring kSysExColumn = kSysEx + 12_u;
  static const ustring kSysExRow = kSysEx + 13_u;
  static const ustring kSysExAll = kSysEx + 14_u;
  static const ustring kSysExFlash = kSysEx + 35_u;
  static const ustring kSysExPulse = kSysEx + 40_u;
  static const ustring kSysExScrollText = kSysEx + 20_u;
  static const ustring kSysExLayout = kSysEx + 34_u;
  static const ustring kSysExFader = kSysEx + 43_u;
}  // namespace

void recv_bind(double timestamp, std::vector<unsigned char>* message,
               void* userData) {
  Launchpad* launchpad = reinterpret_cast<Launchpad*>(userData);
  ustring message_str(message->begin(), message->end());
  launchpad->receive(message_str);
}

Launchpad::Launchpad()
    : m_connected(false) {
  m_output = new RtMidiOut();
  unsigned int nPorts = m_output->getPortCount();

  for (int i = 0; i < nPorts; i++) {
    std::string name = m_output->getPortName(i);
    if (name.compare(0, 9, "Launchpad") == 0) {
      m_connected = true;
      std::cout << "Output : " << name << std::endl;
      m_output->openPort(i);
    }
  }
  setAllLed(0);

  m_input = new RtMidiIn();

  // Check available ports.
  nPorts = m_input->getPortCount();

  for (int i = 0; i < nPorts; i++) {
    std::string name = m_input->getPortName(i);
    if (name.compare(0, 9, "Launchpad") == 0) {
      m_connected = true;
      m_input->openPort(i);
      m_input->setCallback(&recv_bind, this);
    }
  }
}

Launchpad::~Launchpad() {
  delete m_output;
  delete m_input;
}

void Launchpad::receive(const ustring& message) {
  if (message.size() < 3) return;
  uint8_t channel = message.at(0);
  uint8_t note    = message.at(1);
  uint8_t command = message.at(2);

  switch (command) {
    case 127:
      if (note == 104) {
        changeMapping();
      } else {
        noteOn(channel, note);
      }
      break;
    case 0:
      noteOff(channel, note);
      break;
  }
}

void Launchpad::send(const ustring& message) {
  std::vector<unsigned char> buffer(message.begin(), message.end());
  m_output->sendMessage(&buffer);
}

void Launchpad::setLed(uint8_t note, uint8_t red, uint8_t green,
                          uint8_t blue) {
  if (!m_transactional) {
    m_message = kSysExLedRGB;
  }
  m_message += note;
  m_message += red   * 63.0 / 255;
  m_message += green * 63.0 / 255;
  m_message += blue  * 63.0 / 255;

  if (!m_transactional && isConnected()) {
    m_message += kSysExEnd;
    send(m_message);
  }
}

void Launchpad::setLed(uint8_t x, uint8_t y, uint8_t color) {
  if (x < 1 || x > 8) return;
  if (y < 1 || y > 8) return;
  uint8_t note = 10 * y + x;
  if (isConnected()) {
    send(kNoteOn + note + color);
  }
}

void Launchpad::setLed(uint8_t x, uint8_t y, uint8_t red,
                          uint8_t green, uint8_t blue) {
  if (x < 1 || x > 9) return;
  if (y < 1 || y > 9) return;
  setLed(10 * y + x, red, green, blue);
}

void Launchpad::setLed(uint8_t note, Color color) {
  setLed(note, color.red, color.green, color.blue);
}

void Launchpad::setLed(uint8_t x, uint8_t y, Color color) {
  setLed(x, y, color.red, color.green, color.blue);
}

void Launchpad::setAllLed(uint8_t color) {
  if (isConnected()) {
    send(kSysExAll + color + kSysExEnd);
  }
}

void Launchpad::flashLed(uint8_t note, Color color) {
  setLed(note, color);
  //  CC message for top row, regular for all others
  ustring channel = note > 100 ? kCCFlash : kFlash;
  if (isConnected()) {
    send(channel + note + 0_u);
  }
}

void Launchpad::pulseLed(uint8_t x, uint8_t y, Color color) {
  if (color == Color::Red) {
    pulseLed(10 * y + x, 5);
  } else if (color == Color::Blue) {
    pulseLed(10 * y + x, 45);
  } else {
    pulseLed(10 * y + x, 30);
  }
}

void Launchpad::pulseLed(uint8_t note, uint8_t color) {
  //  CC message for top row, regular for all others
  ustring channel = note > 100 ? kCCPulse : kPulse;
  if (isConnected()) {
    send(channel + note + color);
  }
}

void Launchpad::beginTransaction() {
  m_message.clear();
  m_message = kSysExLedRGB;
  m_transactional = true;
}

void Launchpad::commitTransaction() {
  m_message += kSysExEnd;
  if (isConnected()) {
      send(m_message);
  }
  m_transactional = false;
}

void Launchpad::scrollText(const std::string& text) {
  ustring utext(text.begin(), text.end());
  if (isConnected()) {
    send(kSysExScrollText + utext + kSysExEnd);
  }
}


