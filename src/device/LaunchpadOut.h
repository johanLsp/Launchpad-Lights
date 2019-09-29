// Copyright 2019 Johan Lasperas
#ifndef SRC_DEVICE_LAUNCHPADOUT_H_
#define SRC_DEVICE_LAUNCHPADOUT_H_

#include <cstdint>
#include <string>
#include <vector>
#include "rtmidi/RtMidi.h"
#include "Color.h"

class LaunchpadOut {
 public:
  LaunchpadOut();
  ~LaunchpadOut();

  void setAllLed(int color);
  void setLed(uint8_t x, uint8_t y, uint8_t color);
  void setLed(uint8_t x, uint8_t y, Color color);
  void setLed(uint8_t note, Color color);
  void setLed(uint8_t note, uint8_t red, uint8_t green, uint8_t blue);
  void setLed(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue);
  void flashLed(int note, Color color);

  void pulseLed(int x, int y, Color color);
  void pulseLed(int note, int color);
  void scrollText(std::string text);

  void beginTransaction();
  void commitTransaction();

  bool isConnected() { return m_connected; }

 private:
  RtMidiOut* m_output;
  bool m_transactional = false;
  bool m_connected;
  std::vector<unsigned char> m_message;
};

#endif  // SRC_DEVICE_LAUNCHPADOUT_H_
