#include "mapping/page/SequencerPage.h"

SequencerPage::SequencerPage(LaunchpadOut* output)
:output(output) {
    palette[0] = Color::Red;
    palette[1] = Color::Green;
    palette[2] = Color::Blue;
    palette[3] = Color::Cyan;
    palette[4] = Color::Magenta;
    palette[5] = Color::Yellow;
    palette[6] = Color::Black;
    palette[7] = Color::White;
}

SequencerPage::~SequencerPage() {
}

void SequencerPage::refresh() {
    output->beginTransaction();

    for (int deck = 0; deck < 2; deck++) {
        for (int index = 0; index < 32; index++) {
            output->setLed(getNote(deck, index), colors[deck][index].dim());
        }
    }

    for (int i = 0; i < 8; i++) {
        if (i == currentColorIdx) {
            output->setLed(9, i + 1, palette[i]);
        } else {
            output->setLed(9, i + 1, palette[i].dim());
        }
    }

    output->commitTransaction();
}

bool SequencerPage::noteOn(int note) {
    // Buttons on the right
    if (note%10 == 9) {
        output->setLed(9, currentColorIdx + 1, getCurrentColor().dim());
        currentColorIdx = note / 10 - 1;
        output->setLed(9, currentColorIdx + 1, getCurrentColor());
    } else {
        int deck = getDeck(note);
        int index = getIndex(note);

        setColor(deck, index);
        output->setLed(note, colors[deck][index].dim());
    }

    return true;
}

void SequencerPage::setColor(int deck, int index) {
    if (colors[deck][index] != getCurrentColor()) {
        colors[deck][index] = getCurrentColor();
    } else {
        colors[deck][index] = Color::Black;
    }
}

void SequencerPage::setColor(int deck, int index, Color color) {
    colors[deck][index] = color;
}

void SequencerPage::setCurrent(int index) {
    int previous = (index == 0 ? 31 : index-1);

    output->setLed(getNote(currentDeck, previous),
                   colors[currentDeck][previous].dim());

    if (colors[currentDeck][index] == Color::Black) {
        output->setLed(getNote(currentDeck, index), Color::Grey);
    } else {
        output->setLed(getNote(currentDeck, index), colors[currentDeck][index]);
    }
}

void SequencerPage::speedUp() {
    if (speed == TICKS_PER_BEAT) return;
    speed *= 2;
}

void SequencerPage::speedDown() {
    if (speed == 1) return;
    speed /= 2;
}

void SequencerPage::switchDeck() {
    currentDeck = 1 - currentDeck;
}

int SequencerPage::getNote(int deck, int index) {
    int x_idx = index % 8 + 1;
    int y_idx = 4 - index / 8 +(1 - deck) * 4;
    return 10 * y_idx + x_idx;
}

int SequencerPage::getDeck(int note) {
    return note > 50 ? 0 : 1;
}

int SequencerPage::getIndex(int note) {
    int x_idx = note % 10 - 1;
    int y_idx = ((note - 10) % 40) / 10;
    return x_idx + 8 * (3 - y_idx);
}
