#include "Page.h"

Page::Page(LaunchpadOut* output)
:output(output) {
}

Page::~Page() {
}

void Page::setCurrentColor(Color color) {
    this->currentColor = color;
}
