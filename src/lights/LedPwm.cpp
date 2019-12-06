// Copyright 2019 Johan Lasperas
#include "LedPwm.h"

#include <cstring>
#include <iostream>

#include <wiringPi.h>
#include <softPwm.h>

namespace {
    constexpr int kRedPin   = 0;
    constexpr int kGreenPin = 9;
    constexpr int kBluePin  = 13;
}

LedPwm::LedPwm() {
  m_wiringPiSupport = true;
  FILE *cpuFd;
  char line[120];

  if ((cpuFd = fopen("/proc/cpuinfo", "r")) == NULL) {
    m_wiringPiSupport = false;
  } else {
    while (fgets(line, 120, cpuFd) != NULL) {
      if (strncmp(line, "Hardware", 8) == 0) break;
    }

    if (strncmp(line, "Hardware", 8) != 0) {
      m_wiringPiSupport = false;
    }
  }
  if (!m_wiringPiSupport) {
    std::cout << "No wiringPi support" << std::endl;
    return;
  }
  wiringPiSetup();
  softPwmCreate(kRedPin,   0, 100);
  softPwmCreate(kGreenPin, 0, 100);
  softPwmCreate(kBluePin,  0, 100);
}

void LedPwm::setColor(uint8_t red, uint8_t green, uint8_t blue) {
  if (!m_wiringPiSupport) return;

  softPwmWrite(kRedPin,   getIntensity() * 100.0 / 255 * red);
  softPwmWrite(kGreenPin, getIntensity() * 100.0 / 255 * green);
  softPwmWrite(kBluePin,  getIntensity() * 100.0 / 255 * blue);
}
