// Copyright 2019 Johan Lasperas
#ifndef SRC_DEVICE_LAUNCHPAD_H_
#define SRC_DEVICE_LAUNCHPAD_H_

#include <cstdint>
#include <string>
#include "device/Device.h"
#include "rtmidi/RtMidi.h"
#include "util/Color.h"
#include "util/UString.h"

class Launchpad : public Device {
 public:
  explicit Launchpad(Transport* transport);

  void receive(Transport::Type type, const ustring& message) override;

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

 private:
  bool m_transactional = false;
  ustring m_message;
};

#endif  // SRC_DEVICE_LAUNCHPAD_H_
