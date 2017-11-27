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

 private:
    bool wiringPiSupport;
};

#endif  // INCLUDE_STRIPE_H_
