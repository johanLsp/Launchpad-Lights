// Copyright 2019 Johan Lasperas
#ifndef SRC_COLOR_H_
#define SRC_COLOR_H_

#include <cstdint>

class Color {
 public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    Color():red(0), green(0), blue(0) {}
    Color(uint8_t red, uint8_t green, uint8_t blue)
        :red(red), green(green), blue(blue) {}

  bool operator==(Color rhs) {
    return rhs.red == red && rhs.green == green && rhs.blue == blue;
  }
  bool operator!=(Color rhs) {
    return !(*this == rhs);
  }
  Color dim() {
    return Color(red/10, green/10, blue/10);
  }

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

#endif  // SRC_COLOR_H_
