// Copyright 2019 Johan Lasperas
#include "device/LaunchpadOut.h"

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

LaunchpadOut::LaunchpadOut()
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
}

LaunchpadOut::~LaunchpadOut() {
  delete m_output;
}

void LaunchpadOut::send(const ustring& message) {
  std::vector<unsigned char> buffer(message.begin(), message.end());
  m_output->sendMessage(&buffer);
}

void LaunchpadOut::setLed(uint8_t note, uint8_t red, uint8_t green,
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

void LaunchpadOut::setLed(uint8_t x, uint8_t y, uint8_t color) {
  if (x < 1 || x > 8) return;
  if (y < 1 || y > 8) return;
  uint8_t note = 10 * y + x;
  if (isConnected()) {
    send(kNoteOn + note + color);
  }
}

void LaunchpadOut::setLed(uint8_t x, uint8_t y, uint8_t red,
                          uint8_t green, uint8_t blue) {
  if (x < 1 || x > 9) return;
  if (y < 1 || y > 9) return;
  setLed(10 * y + x, red, green, blue);
}

void LaunchpadOut::setLed(uint8_t note, Color color) {
  setLed(note, color.red, color.green, color.blue);
}

void LaunchpadOut::setLed(uint8_t x, uint8_t y, Color color) {
  setLed(x, y, color.red, color.green, color.blue);
}

void LaunchpadOut::setAllLed(uint8_t color) {
  if (isConnected()) {
    send(kSysExAll + color + kSysExEnd);
  }
}

void LaunchpadOut::flashLed(uint8_t note, Color color) {
  setLed(note, color);
  //  CC message for top row, regular for all others
  ustring channel = note > 100 ? kCCFlash : kFlash;
  if (isConnected()) {
    send(channel + note + 0_u);
  }
}

void LaunchpadOut::pulseLed(uint8_t x, uint8_t y, Color color) {
  if (color == Color::Red) {
    pulseLed(10 * y + x, 5);
  } else if (color == Color::Blue) {
    pulseLed(10 * y + x, 45);
  } else {
    pulseLed(10 * y + x, 30);
  }
}

void LaunchpadOut::pulseLed(uint8_t note, uint8_t color) {
  //  CC message for top row, regular for all others
  ustring channel = note > 100 ? kCCPulse : kPulse;
  if (isConnected()) {
    send(channel + note + color);
  }
}

void LaunchpadOut::beginTransaction() {
  m_message.clear();
  m_message = kSysExLedRGB;
  m_transactional = true;
}

void LaunchpadOut::commitTransaction() {
  m_message += kSysExEnd;
  if (isConnected()) {
      send(m_message);
  }
  m_transactional = false;
}

void LaunchpadOut::scrollText(const std::string& text) {
  ustring utext(text.begin(), text.end());
  if (isConnected()) {
    send(kSysExScrollText + utext + kSysExEnd);
  }
}
