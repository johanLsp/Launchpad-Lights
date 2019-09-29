#include "mapping/DirectMapping.h"

DirectMapping::DirectMapping(LaunchpadOut* output, Stripe* stripe)
:output(output), stripe(stripe) {
}

void DirectMapping::start() {
    output->setAllLed(0);
    for (unsigned char r = 0; r < 4; r++) {
        for (unsigned char g = 0; g < 4; g++) {
            for (unsigned char b = 0; b < 4; b++) {
                int x = 4 * (r % 2) + b + 1;
                int y = 4 * (r / 2) + g + 1;
                output->setLed(x,
                               y,
                               255.0 * r / 3,
                               255.0 * g / 3,
                               255.0 * b / 3);
            }
        }
    }

    output->pulseLed(NOTE_MAPPING, 5);
    num_pressed = 0;
}

void DirectMapping::stop() {
    num_pressed = 0;
    output->setAllLed(0);
}


void DirectMapping::noteOn(int channel, int note) {
    num_pressed++;
    int x = note % 10 - 1;
    int y = note / 10 - 1;
    int r = 2 * floor(y / 4) + floor(x / 4);
    int g = y % 4;
    int b = x % 4;
    stripe->setColor(255.0 * r / 3, 255.0 * g / 3, 255.0 * b / 3);
}

void DirectMapping::noteOff(int channel, int note) {
    if (note > 100) return;
    num_pressed--;
    if (num_pressed == 0) {
        stripe->setColor(0, 0, 0);
    }
}

DirectMapping::~DirectMapping() {
}
