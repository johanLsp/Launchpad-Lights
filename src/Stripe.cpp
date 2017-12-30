#include "Stripe.h"

namespace {
    constexpr int kRedPin   = 0;
    constexpr int kGreenPin = 9;
    constexpr int kBluePin  = 13;
}

Stripe::Stripe() {
    wiringPiSupport = true;
    FILE *cpuFd;
    char line[120];

    if ((cpuFd = fopen("/proc/cpuinfo", "r")) == NULL) {
        wiringPiSupport = false;
    } else {
        while (fgets(line, 120, cpuFd) != NULL) {
            if (strncmp(line, "Hardware", 8) == 0) break;
        }

        if (strncmp(line, "Hardware", 8) != 0) {
            wiringPiSupport = false;
        }
    }

    if (!wiringPiSupport) {
        std::cout << "No wiringPi support" << std::endl;
        return;
    }

    wiringPiSetup();
    softPwmCreate(kRedPin,   0, 100);
    softPwmCreate(kGreenPin, 0, 100);
    softPwmCreate(kBluePin,  0, 100);
}

Stripe::~Stripe() {
}

void Stripe::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    if (!wiringPiSupport) return;

    softPwmWrite(kRedPin,   100.0 / 255 * red);
    softPwmWrite(kGreenPin, 100.0 / 255 * green);
    softPwmWrite(kBluePin,  100.0 / 255 * blue);
}


void Stripe::setColor(Color color) {
    setColor(color.red, color.green, color.blue);
}
