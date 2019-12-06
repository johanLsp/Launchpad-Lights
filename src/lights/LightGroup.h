// Copyright 2019 Johan Lasperas
#ifndef SRC_LIGHTS_LIGHTGROUP_H_
#define SRC_LIGHTS_LIGHTGROUP_H_

#include <cstdint>
#include <vector>
#include "lights/Light.h"

class LightGroup : public Light {
 public:
  LightGroup() {}

  void setColor(uint8_t red, uint8_t green, uint8_t blue) override {
    for (Light* light : m_lights) {
      light->setColor(red, green, blue);
    }
  }
  // Un-hide setColor
  using Light::setColor;

  void addLight(Light* light) { m_lights.push_back(light); }
 private:
  std::vector<Light*> m_lights;
};

#endif  // SRC_LIGHTS_LIGHTGROUP_H_
