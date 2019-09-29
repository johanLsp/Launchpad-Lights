#include "ServerIn.h"
#include <iostream>

using nlohmann::json;

ServerIn::ServerIn() {
    server = zsock_new_sub("tcp://localhost:7050", "Colors");
}

void ServerIn::run() {
    running = true;
    while (running) {
        receive();
        sleep(0.5);
    }
}

void ServerIn::stop() {
    running = false;
}

void ServerIn::receive() {
    std::cout << "Receiving" << std::endl;
    char *message = zstr_recv(server);
    std::cout << message << std::endl;

    std::vector<Color> colors;
    json j = json::parse(message+7);
    for (auto& c : j) {
      Color color(c["red"], c["green"], c["blue"]);
      colors.push_back(color);
    }

    mappings[currentMapping]->setColors(colors);
    zstr_free(&message);
}

void ServerIn::addMapping(Mapping* mapping) {
    mappings.push_back(mapping);
    if (mappings.size() == 1)
        mappings[0]->start();
}

void ServerIn::changeMapping() {
    mappings[currentMapping]->stop();
    currentMapping++;
    if (currentMapping >= mappings.size())
        currentMapping = 0;

    mappings[currentMapping]->start();
}

ServerIn::~ServerIn() {
    zsock_destroy(&server);
}
