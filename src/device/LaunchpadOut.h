// Copyright 2019 Johan Lasperas
#ifndef SRC_DEVICE_LAUNCHPADOUT_H_
#define SRC_DEVICE_LAUNCHPADOUT_H_

#include <cstdint>
#include <string>
#include <vector>
#include "rtmidi/RtMidi.h"
#include "Color.h"
#include "util/UString.h"

class LaunchpadOut {
 public:
  LaunchpadOut();
  ~LaunchpadOut();


  void setAllLed(uint8_t color);
  void setLed(uint8_t x, uint8_t y, uint8_t color);
  void setLed(uint8_t x, uint8_t y, Color color);
  void setLed(uint8_t note, Color color);
  void setLed(uint8_t note, uint8_t red, uint8_t green, uint8_t blue);
  void setLed(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue);
  void flashLed(uint8_t note, Color color);

  void pulseLed(uint8_t x, uint8_t y, Color color);
  void pulseLed(uint8_t note, uint8_t color);
  void scrollText(const std::string& text);

  void beginTransaction();
  void commitTransaction();

  bool isConnected() { return m_connected; }

 private:
  void send(const ustring& message);

  RtMidiOut* m_output;
  bool m_transactional = false;
  bool m_connected;
  ustring m_message;
};

#endif  // SRC_DEVICE_LAUNCHPADOUT_H_
