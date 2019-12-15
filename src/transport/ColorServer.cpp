// Copyright 2019 Johan Lasperas
#include "transport/ColorServer.h"

#include <iostream>
#include <vector>

ColorServer::ColorServer()
    : m_running(false) {
  m_server = zsock_new_sub("tcp://localhost:7050", "Colors");
  m_connected = true;
}

void ColorServer::run() {
  m_running = true;
  while (m_running) {
    zpoller_t* poller = zpoller_new(m_server, NULL);
    if (!zpoller_wait(poller, 200)) continue;
    char* message = zstr_recv(m_server);
    receive(message);
    zstr_free(&message);
  }
}

void ColorServer::start() {
  if (m_running) return;
  m_thread = new std::thread(&ColorServer::run, this);
}

void ColorServer::stop() {
  if (!m_running) return;
  m_running = false;
  m_thread->join();
  delete m_thread;
}

ColorServer::~ColorServer() {
  zsock_destroy(&m_server);
}
