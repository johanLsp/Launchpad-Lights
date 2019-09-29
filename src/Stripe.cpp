// Copyright 2019 Johan Lasperas
#include "Stripe.h"

namespace {
    constexpr int kRedPin   = 0;
    constexpr int kGreenPin = 9;
    constexpr int kBluePin  = 13;
}

Stripe::Stripe()
    : m_intensity(1.0) {
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

void Stripe::setColor(uint8_t red, uint8_t green, uint8_t blue) {
  if (!m_wiringPiSupport) return;

  softPwmWrite(kRedPin,   m_intensity * 100.0 / 255 * red);
  softPwmWrite(kGreenPin, m_intensity * 100.0 / 255 * green);
  softPwmWrite(kBluePin,  m_intensity * 100.0 / 255 * blue);
}

void Stripe::setColor(Color color) {
  setColor(color.red, color.green, color.blue);
}

void Stripe::setIntensity(double intensity) {
  m_intensity = intensity;
}
