// Copyright 2019 Johan Lasperas
#ifndef SRC_MAPPING_DIRECTMAPPING_H_
#define SRC_MAPPING_DIRECTMAPPING_H_

#include "lights/Light.h"
#include "mapping/Mapping.h"

class DirectMapping : public Mapping {
 public:
  DirectMapping(Launchpad* launchpad, Light* light);
  void noteOn(int channel, int note) override;
  void noteOff(int channel, int note) override;
  void setColors(const std::vector<Color>& colors) override;
  void start() override;
  void stop() override;
  void refresh() override;

 private:
    int m_num_pressed;
};

#endif  // SRC_MAPPING_DIRECTMAPPING_H_
