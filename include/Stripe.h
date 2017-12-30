#ifndef INCLUDE_STRIPE_H_
#define INCLUDE_STRIPE_H_

#include <cstdint>
#include <cstring>
#include <iostream>

#include <wiringPi.h>
#include <softPwm.h>

#include "Color.h"

class Stripe {
 public:
    Stripe();
    ~Stripe();

    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setColor(Color color);
    double getIntensity() { return m_intensity;}
    void setIntensity(double intensity);

 private:
    bool wiringPiSupport;
    double m_intensity;
};

#endif  // INCLUDE_STRIPE_H_
