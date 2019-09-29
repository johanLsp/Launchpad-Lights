// Copyright 2019 Johan Lasperas
#include "mapping/page/ColorPage.h"

#include <cmath>

void ColorPage::refresh() {
  for (unsigned char r = 0; r < 4; r++) {
    for (unsigned char g = 0; g < 4; g++) {
      for (unsigned char b = 0; b < 4; b++) {
          uint8_t x = 4 *(r % 2) + b + 1;
          uint8_t y = 4 *(r / 2) + g + 1;
          uint8_t red = 255.0 * r / 3;
          uint8_t green = 255.0 * g / 3;
          uint8_t blue = 255.0 * b / 3;
          m_output->setLed(x, y, red, green, blue);
      }
    }
  }
}

bool ColorPage::noteOn(int note) {
  int x = note % 10 - 1;
  int y = note / 10 - 1;

  if (x > 7 || y > 8) return true;

  int r = 2 * floor(y / 4) + floor(x / 4);
  int g = y % 4;
  int b = x % 4;
  m_color = Color(255.0 * r / 3, 255.0 * g / 3, 255.0 * b / 3);

  return false;
}
