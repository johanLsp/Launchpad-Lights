#ifndef INCLUDE_MAPPING_H_
#define INCLUDE_MAPPING_H_

#include "device/LaunchpadOut.h"
#include "Stripe.h"

#define NOTE_MAPPING 104

class Mapping {
 public:
    Mapping();
    Mapping(LaunchpadOut* output, Stripe* stripe);
    ~Mapping();

    virtual void noteOn(int channel, int note) = 0;
    virtual void noteOff(int channel, int note) = 0;
    virtual void setColors(std::vector<Color>& colors) = 0;

    virtual void start() = 0;
    virtual void stop() = 0;

 private:
    LaunchpadOut* output;
    Stripe* stripe;
};

#endif  // INCLUDE_MAPPING_H_
