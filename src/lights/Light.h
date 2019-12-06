// Copyright 2019 Johan Lasperas
#ifndef SRC_LIGHTS_LIGHT_H_
#define SRC_LIGHTS_LIGHT_H_

#include <cstdint>
#include "util/Color.h"

class Light {
 public:
  Light() : m_intensity(1.0) {}

  virtual void setColor(uint8_t red, uint8_t green, uint8_t blue) = 0;

  void setColor(const Color& color) { setColor(color.red, color.green, color.blue); }
  double getIntensity() { return m_intensity; }
  void setIntensity(double intensity) { m_intensity = intensity; }

 private:
  double m_intensity;
};

#endif  // SRC_LIGHTS_LIGHT_H_
