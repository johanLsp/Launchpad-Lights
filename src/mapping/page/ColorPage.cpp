#include "mapping/page/ColorPage.h"

ColorPage::ColorPage(LaunchpadOut* output)
:output(output) {
}

ColorPage::~ColorPage() {
}

void ColorPage::refresh() {
    for (unsigned char r = 0; r < 4; r++) {
        for (unsigned char g = 0; g < 4; g++) {
            for (unsigned char b = 0; b < 4; b++) {
                int x = 4 *(r % 2) + b + 1;
                int y = 4 *(r / 2) + g + 1;
                output->setLed(x,
                               y,
                               255.0 * r / 3,
                               255.0 * g / 3,
                               255.0 * b / 3);
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
    currentColor = Color(255.0 * r / 3, 255.0 * g / 3, 255.0 * b / 3);

    return false;
}
