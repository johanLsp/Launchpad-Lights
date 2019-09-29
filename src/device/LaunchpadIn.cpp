#include "device/LaunchpadIn.h"

void recv_bind(double                      deltatime,
               std::vector<unsigned char>* message,
               void*                       userData) {
    LaunchpadIn* launchpad = reinterpret_cast<LaunchpadIn*>(userData);
    launchpad->receive(deltatime, message, userData);
}



LaunchpadIn::LaunchpadIn() {
    input = new RtMidiIn();
    connected = false;

    // Check available ports.
    unsigned int nPorts = input->getPortCount();

    for (int i = 0; i < nPorts; i++) {
        std::string name = input->getPortName(i);
        if (name.compare(0, 9, "Launchpad") == 0) {
            connected = true;
            input->openPort(i);
            input->setCallback(&recv_bind, this);
        }
    }
}

void LaunchpadIn::addMapping(Mapping* mapping) {
    mappings.push_back(mapping);
    if (mappings.size() == 1)
        mappings[0]->start();
}

void LaunchpadIn::receive(double                      deltatime,
                          std::vector<unsigned char>* message,
                          void*                       userData) {
    unsigned int nBytes = message->size();
    int channel = message->at(0);
    int note    = message->at(1);
    int command = message->at(2);

    switch (command) {
     case 127:
        if (note == 104) {
            changeMapping();
        } else {
            mappings[currentMapping]->noteOn(channel, note);
        }
        break;
     case 0:
        mappings[currentMapping]->noteOff(channel, note);
        break;
    }
}


void LaunchpadIn::changeMapping() {
    mappings[currentMapping]->stop();
    currentMapping++;
    if (currentMapping >= mappings.size())
        currentMapping = 0;

    mappings[currentMapping]->start();
}

LaunchpadIn::~LaunchpadIn() {
}
