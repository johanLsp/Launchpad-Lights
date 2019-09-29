// Copyright 2019 Johan Lasperas
#ifndef SRC_MAPPING_MAPPING_H_
#define SRC_MAPPING_MAPPING_H_

#include <vector>

#include "device/LaunchpadOut.h"
#include "Stripe.h"

#define NOTE_MAPPING 104

class Mapping {
 public:
  Mapping(LaunchpadOut* output, Stripe* stripe)
      : m_output(output), m_stripe(stripe) {}

  virtual void noteOn(int channel, int note) = 0;
  virtual void noteOff(int channel, int note) = 0;
  virtual void setColors(const std::vector<Color>& colors) = 0;
  virtual void start() = 0;
  virtual void stop() = 0;

 protected:
  LaunchpadOut* m_output;
  Stripe* m_stripe;
};

#endif  // SRC_MAPPING_MAPPING_H_
