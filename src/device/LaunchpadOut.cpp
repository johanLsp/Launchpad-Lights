#include "device/LaunchpadOut.h"

LaunchpadOut::LaunchpadOut() {
    output = new RtMidiOut();
    connected = false;
    unsigned int nPorts = output->getPortCount();

    for (int i = 0; i < nPorts; i++) {
        std::string name = output->getPortName(i);
        if (name.compare(0, 9, "Launchpad") == 0) {
            connected = true;
            std::cout << "Output : " << name << std::endl;
            output->openPort(i);
        }
    }
    setAllLed(0);
}



void LaunchpadOut::setLed(uint8_t note, uint8_t red, uint8_t green, uint8_t blue) {
    if (!transactional) {
        message.clear();
        message.push_back(240);
        message.push_back(0);
        message.push_back(32);
        message.push_back(41);
        message.push_back(2);
        message.push_back(24);
        message.push_back(11);
    }

    message.push_back(note);
    message.push_back(red   * 63.0 / 255);
    message.push_back(green * 63.0 / 255);
    message.push_back(blue  * 63.0 / 255);

    if (!transactional && isConnected()) {
        message.push_back(247);
        output->sendMessage(&message);
    }
}

void LaunchpadOut::setLed(uint8_t x, uint8_t y, uint8_t color) {
    if (x < 1 || x > 8) return;
    if (y < 1 || y > 8) return;

    std::vector<unsigned char> message;
    message.push_back(144);
    message.push_back(10 * y + x);
    message.push_back(color);
    if (isConnected()) {
        output->sendMessage(&message);
    }
}

void LaunchpadOut::setLed(uint8_t x,
                          uint8_t y,
                          uint8_t red,
                          uint8_t green,
                          uint8_t blue) {
    if (x < 1 || x > 9) return;
    if (y < 1 || y > 9) return;

    setLed(10 * y + x, red, green, blue);
}

void LaunchpadOut::setLed(uint8_t note, Color color) {
    setLed(note, color.red, color.green, color.blue);
}

void LaunchpadOut::setLed(uint8_t x, uint8_t y, Color color) {
    setLed(x, y, color.red, color.green, color.blue);
}


void LaunchpadOut::setAllLed(int color) {
    std::vector<unsigned char> message;
    message.push_back(240);
    message.push_back(0);
    message.push_back(32);
    message.push_back(41);
    message.push_back(2);
    message.push_back(24);
    message.push_back(14);
    message.push_back(color);
    message.push_back(247);
    if (isConnected()) {
        output->sendMessage(&message);
    }
}

void LaunchpadOut::flashLed(int note, Color color) {
    setLed(note, color);
    int channel;
    // CC (177) message for top row, NoteOn(145) for all others
    if (note > 100) {
        channel = 177;
    } else {
        channel = 145;
    }
    std::vector<unsigned char> message;
    message.push_back(channel);
    message.push_back(note);
    message.push_back(0);
    if (isConnected()) {
        output->sendMessage(&message);
    }
}

void LaunchpadOut::pulseLed(int x, int y, Color color) {
    if (color == Color::Red) {
        pulseLed(10 * y + x, 5);
    } else if (color == Color::Blue) {
        pulseLed(10 * y + x, 45);
    } else {
        pulseLed(10 * y + x, 30);
    }
}


void LaunchpadOut::pulseLed(int note, int color) {
    int channel;
    //  CC (178) message for top row, NoteOn(146) for all others
    if (note > 100) {
        channel = 178;
    } else {
        channel = 146;
    }
    std::vector<unsigned char> message;
    message.push_back(channel);
    message.push_back(note);
    message.push_back(color);
    if (isConnected()) {
        output->sendMessage(&message);
    }
}

void LaunchpadOut::beginTransaction() {
    message.clear();
    message.push_back(240);
    message.push_back(0);
    message.push_back(32);
    message.push_back(41);
    message.push_back(2);
    message.push_back(24);

    message.push_back(11);

    transactional = true;
}

void LaunchpadOut::commitTransaction() {
    message.push_back(247);
    if (isConnected()) {
        output->sendMessage(&message);
    }
    transactional = false;
}


void LaunchpadOut::scrollText(std::string text) {
    std::vector<unsigned char> message;
    message.push_back(240);
    message.push_back(0);
    message.push_back(32);
    message.push_back(41);
    message.push_back(2);
    message.push_back(24);
    message.push_back(20);

    message.push_back(5);
    message.push_back(0);

    for (int i = 0; i < text.size(); i++) {
        message.push_back(text[i]);
    }

    message.push_back(247);
    if (isConnected()) {
        output->sendMessage(&message);
    }
}

LaunchpadOut::~LaunchpadOut() {
}
