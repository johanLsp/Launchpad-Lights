// Copyright 2019 Johan Lasperas
#include "device/Server.h"

#include <iostream>
#include <vector>

#include "json.hpp"
#include "util/Color.h"

using nlohmann::json;

Server::Server()
    : m_running(false) {
    m_server = zsock_new_sub("tcp://localhost:7050", "Colors");
}

void Server::run() {
    m_running = true;
    while (m_running) {
        receive();
        sleep(0.5);
    }
}

void Server::stop() {
    m_running = false;
}

void Server::receive() {
    char *message = zstr_recv(m_server);
    std::cout << message << std::endl;

    std::vector<Color> colors;
    json j = json::parse(message+7);
    for (auto& c : j) {
      Color color(c["red"], c["green"], c["blue"]);
      colors.push_back(color);
    }

    setColors(colors);
    zstr_free(&message);
}

Server::~Server() {
    zsock_destroy(&m_server);
}
