// Copyright 2019 Johan Lasperas
#include "mapping/DirectMapping.h"

#include <functional>
#include <cmath>

DirectMapping::DirectMapping(LaunchpadOut* output, Stripe* stripe)
    : Mapping(output, stripe), m_num_pressed(0) {}

void DirectMapping::start() {
  m_output->setAllLed(0);
  for (uint8_t r = 0; r < 4; r++) {
    for (uint8_t g = 0; g < 4; g++) {
      for (uint8_t b = 0; b < 4; b++) {
        uint8_t x = 4 * (r % 2) + b + 1;
        uint8_t y = 4 * (r / 2) + g + 1;
        uint8_t red = 255.0 * r / 3;
        uint8_t green = 255.0 * g / 3;
        uint8_t blue = 255.0 * b / 3;
        m_output->setLed(x, y, red, green, blue);
      }
    }
  }
  m_output->pulseLed(NOTE_MAPPING, 5);
  m_num_pressed = 0;
}

void DirectMapping::stop() {
  m_num_pressed = 0;
  m_output->setAllLed(0);
}

void DirectMapping::noteOn(int channel, int note) {
  m_num_pressed++;
  int x = note % 10 - 1;
  int y = note / 10 - 1;
  int r = 2 * floor(y / 4) + floor(x / 4);
  int g = y % 4;
  int b = x % 4;
  m_stripe->setColor(255.0 * r / 3, 255.0 * g / 3, 255.0 * b / 3);
}

void DirectMapping::noteOff(int channel, int note) {
  if (note > 100) return;
  m_num_pressed--;
  if (m_num_pressed == 0) {
    m_stripe->setColor(0, 0, 0);
  }
}
