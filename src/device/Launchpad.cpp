// Copyright 2019 Johan Lasperas
#include "device/Launchpad.h"

#include <functional>
#include <sstream>

#include "json.hpp"

using nlohmann::json;

namespace {
  // See Launchpad MKII Programmer's Reference Manual.
  static const std::string kNoteOff = {static_cast<char>(128)};
  static const std::string kNoteOn = {static_cast<char>(144)};
  static const std::string kFlash = {static_cast<char>(145)};
  static const std::string kPulse = {static_cast<char>(146)};
  static const std::string kCCFlash = {static_cast<char>(177)};
  static const std::string kCCPulse = {static_cast<char>(178)};
  static const std::string kSysEx = {static_cast<char>(240), 0, 32, 41, 2, 24};
  static const std::string kSysExEnd = {static_cast<char>(247)};
  static const std::string kSysExLedStd = kSysEx + static_cast<char>(10);
  static const std::string kSysExLedRGB = kSysEx + static_cast<char>(11);
  static const std::string kSysExColumn = kSysEx + static_cast<char>(12);
  static const std::string kSysExRow = kSysEx + static_cast<char>(13);
  static const std::string kSysExAll = kSysEx + static_cast<char>(14);
  static const std::string kSysExFlash = kSysEx + static_cast<char>(35);
  static const std::string kSysExPulse = kSysEx + static_cast<char>(40);
  static const std::string kSysExScrollText = kSysEx + static_cast<char>(20);
  static const std::string kSysExLayout = kSysEx + static_cast<char>(34);
  static const std::string kSysExFader = kSysEx + static_cast<char>(43);
}  // namespace

Launchpad::Launchpad(Transport* transport)
    : Device(transport) {
  setAllLed(0);
}

void Launchpad::receive(Transport::Type type, const std::string& message) {
  switch (type) {
    case Transport::Type::MIDI: {
      if (message.size() < 3) return;
      uint8_t channel = message.at(0);
      uint8_t note    = message.at(1);
      uint8_t command = message.at(2);

      switch (command) {
        case 105:
          std::cout << "Refresh" << std::endl;
          refresh();
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
      if (colors.size() <= 1) {
        changeMapping(0);
      } else {
	changeMapping(1);
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

  if (!m_transactional) {
    m_message += kSysExEnd;
    send(m_message);
  }
}

void Launchpad::setLed(uint8_t x, uint8_t y, uint8_t color) {
  if (x < 1 || x > 8) return;
  if (y < 1 || y > 8) return;
  uint8_t note = 10 * y + x;
  std::string command = kNoteOn;
  command += note;
  command += color;
  send(command);
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
  std::string command = kSysExAll;
  command += color;
  command += kSysExEnd;
  send(command);
}

void Launchpad::flashLed(uint8_t note, Color color) {
  setLed(note, color);
  //  CC message for top row, regular for all others
  std::string command = note > 100 ? kCCFlash : kFlash;
  command += note;
  command += static_cast<char>(0);
  send(command);
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
  std::string command = note > 100 ? kCCPulse : kPulse;
  command += note;
  command += color;
  send(command);
}

void Launchpad::beginTransaction() {
  m_message.clear();
  m_message = kSysExLedRGB;
  m_transactional = true;
}

void Launchpad::commitTransaction() {
  m_message += kSysExEnd;
  send(m_message);
  m_transactional = false;
}

void Launchpad::scrollText(const std::string& text) {
  std::ostringstream command;
  command << kSysExScrollText << text << kSysExEnd;
  send(command.str());
}


