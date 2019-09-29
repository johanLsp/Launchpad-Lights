// Copyright 2019 Johan Lasperas
#include "device/ServerIn.h"

#include <iostream>
#include <vector>

#include "Color.h"
#include "json.hpp"

using nlohmann::json;

ServerIn::ServerIn()
    : m_running(false) {
    m_server = zsock_new_sub("tcp://localhost:7050", "Colors");
}

void ServerIn::run() {
    m_running = true;
    while (m_running) {
        receive();
        sleep(0.5);
    }
}

void ServerIn::stop() {
    m_running = false;
}

void ServerIn::receive() {
    char *message = zstr_recv(m_server);
    std::cout << message << std::endl;

    std::vector<Color> colors;
    json j = json::parse(message+7);
    for (auto& c : j) {
      Color color(c["red"], c["green"], c["blue"]);
      colors.push_back(color);
    }

    m_mappings[m_mapping_idx]->setColors(colors);
    zstr_free(&message);
}

ServerIn::~ServerIn() {
    zsock_destroy(&m_server);
}
