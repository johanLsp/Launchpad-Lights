
// Copyright 2019 Johan Lasperas
#ifndef SRC_LIGHTS_LIGHTBLUETOOTH_H_
#define SRC_LIGHTS_LIGHTBLUETOOTH_H_

#include <memory>
#include <string>
#include <thread>
#include <boost/process/child.hpp>

#include "lights/Light.h"
#include "util/Color.h"

class LightBluetooth : public Light {
 public:
  LightBluetooth();
  ~LightBluetooth();
  void setColor(uint8_t red, uint8_t green, uint8_t blue) override;
  // Un-hide setColor
  using Light::setColor;

 private:
  bool connect();
  void disconnect();
  void run();

  std::unique_ptr<boost::process::child> m_light_child;
  std::unique_ptr<boost::process::ipstream> m_light_in;
  std::unique_ptr<boost::process::opstream> m_light_out;

  std::unique_ptr<std::thread> m_thread;
  bool m_update;
  bool m_running;

  Color m_currentColor;
  bool m_connected;
};

#endif  // SRC_LIGHTS_LIGHTBLUETOOTH_H_
