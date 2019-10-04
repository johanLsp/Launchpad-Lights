// Copyright 2019 Johan Lasperas
#include "transport/ColorServer.h"

#include <iostream>
#include <thread>
#include <vector>

ColorServer::ColorServer()
    : m_running(false) {
  m_server = zsock_new_sub("tcp://localhost:7050", "Colors");
  m_connected = true;
}

void ColorServer::run() {
  m_running = true;
  while (m_running) {
    char *message = zstr_recv(m_server);
    ustring message_str(reinterpret_cast<unsigned char*>(message));
    receive(message_str);
    zstr_free(&message);
  }
}

void ColorServer::start() {
  std::thread serverThread(&ColorServer::run, this);
}

void ColorServer::stop() {
  m_running = false;
}

ColorServer::~ColorServer() {
  zsock_destroy(&m_server);
}
