#ifndef INCLUDE_DIRECTMAPPING_H_
#define INCLUDE_DIRECTMAPPING_H_

#include "mapping/Mapping.h"
#include <functional>
#include <cmath>

class DirectMapping : public Mapping {
 public:
    DirectMapping(LaunchpadOut* output, Stripe* stripe);
    ~DirectMapping();

    void noteOn(int channel, int note);
    void noteOff(int channel, int note);
    void setColors(std::vector<Color>& colors) {}
    void start();
    void stop();

 private:
    LaunchpadOut* output;
    Stripe* stripe;
    int num_pressed = 0;
};

#endif  // INCLUDE_DIRECTMAPPING_H_
