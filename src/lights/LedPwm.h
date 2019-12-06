// Copyright 2019 Johan Lasperas
#ifndef SRC_LIGHTS_LEDPWM_H_
#define SRC_LIGHTS_LEDPWM_H_

#include <cstdint>
#include "lights/Light.h"

class LedPwm : public Light {
 public:
  LedPwm();

  void setColor(uint8_t red, uint8_t green, uint8_t blue) override;

  // Un-hide setColor
  using Light::setColor;
 private:
  bool m_wiringPiSupport;
};

#endif  // SRC_LIGHTS_LEDPWM_H_
