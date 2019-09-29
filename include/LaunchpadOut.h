#ifndef INCLUDE_LAUNCHPADOUT_H_
#define INCLUDE_LAUNCHPADOUT_H_

#include <cstdint>
#include <functional>
#include <string>
#include <vector>
#include "rtmidi/RtMidi.h"
#include "Color.h"

class LaunchpadOut {
 public:
    LaunchpadOut();
    ~LaunchpadOut();

    void setAllLed(int color);
    void setLed(uint8_t x, uint8_t y, uint8_t color);
    void setLed(uint8_t x, uint8_t y, Color color);
    void setLed(uint8_t note, Color color);
    void setLed(uint8_t note, uint8_t red, uint8_t green, uint8_t blue);
    void setLed(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue);
    void flashLed(int note, Color color);

    void pulseLed(int x, int y, Color color);
    void pulseLed(int note, int color);
    void scrollText(std::string text);

    void beginTransaction();
    void commitTransaction();

    bool isConnected() {return connected;}

 private:
    RtMidiOut *output;
    bool transactional = false;
    bool connected;
    std::vector<unsigned char> message;
};

#endif  // INCLUDE_LAUNCHPADOUT_H_
