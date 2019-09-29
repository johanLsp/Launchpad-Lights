#ifndef INCLUDE_COLOR_H_
#define INCLUDE_COLOR_H_

#include <cstdint>

class Color {
 public:
    Color():red(0), green(0), blue(0) {}
    Color(uint8_t red, uint8_t green, uint8_t blue)
    :red(red), green(green), blue(blue) {}

    Color dim();
    bool operator==(Color rhs);
    bool operator!=(Color rhs);


 public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Cyan;
    static const Color Magenta;
    static const Color Yellow;
    static const Color White;
    static const Color Grey;
};

#endif  // INCLUDE_COLOR_H_
