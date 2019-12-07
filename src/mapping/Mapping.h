// Copyright 2019 Johan Lasperas
#ifndef SRC_MAPPING_MAPPING_H_
#define SRC_MAPPING_MAPPING_H_

#include <vector>

#include "lights/Light.h"
#include "device/Launchpad.h"

#define NOTE_MAPPING 104

class Mapping {
 public:
  Mapping(Launchpad* launchpad, Light* light)
      : m_launchpad(launchpad), m_light(light) {}

  virtual void setLaunchpad(Launchpad* launchpad) {
    m_launchpad = launchpad;
    refresh();
  }

  virtual void noteOn(int channel, int note) = 0;
  virtual void noteOff(int channel, int note) = 0;
  virtual void setColors(const std::vector<Color>& colors) = 0;
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void refresh() = 0;

 protected:
  Launchpad* m_launchpad;
  Light* m_light;
};

#endif  // SRC_MAPPING_MAPPING_H_
