// Copyright 2019 Johan Lasperas
#include "device/Launchpad.h"

#include <functional>

#include "json.hpp"

using nlohmann::json;

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

Launchpad::Launchpad(Transport* transport)
    : Device(transport) {
  setAllLed(0);
}

void Launchpad::receive(Transport::Type type, const ustring& message) {
  switch (type) {
    case Transport::Type::MIDI: {
      if (message.size() < 3) return;
      uint8_t channel = message.at(0);
      uint8_t note    = message.at(1);
      uint8_t command = message.at(2);

      switch (command) {
        case 105:
          std::cout << "Refresh" << std::endl;
          start();
          break;
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
        default:
          std::cout << "Unknown command: " << (int) command << std::endl;
      }
      break;
    }
    case Transport::Type::COLOR: {
      std::vector<Color> colors;
      json j = json::parse(message.c_str() + 7);
      for (auto& c : j) {
        Color color(c["red"], c["green"], c["blue"]);
        colors.push_back(color);
      }
      setColors(colors);
    }
  }
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


