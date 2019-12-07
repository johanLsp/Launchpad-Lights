// Copyright 2019 Johan Lasperas
#include "lights/LedRemote.h"

#include <iostream>

LedRemote::LedRemote() {
  m_socket = zsock_new_stream(">tcp://192.168.0.50:5577");

  connect();
  m_currentColor = Color::Black;
}

bool LedRemote::connect() {
  if (m_connected) return true;

  zpoller_t* poller = zpoller_new(m_socket, NULL);
  if (!zpoller_wait(poller, 200)) return false;

  zmsg_t* msg = zmsg_recv(m_socket);
  if (msg) {
    for (int i = 0; i < zmsg_size(msg); i++) {
      zframe_t* frame = zmsg_pop(msg);
      byte* data = zframe_data(frame);
      m_identity = std::string(reinterpret_cast<const char*>(data),
                              zframe_size(frame));
    }
    zmsg_destroy(&msg);
    m_connected = true;
  }
  return m_connected;
}

LedRemote::~LedRemote() {
  zsock_destroy(&m_socket);
}

void LedRemote::setColor(uint8_t red, uint8_t green, uint8_t blue) {
  if (m_currentColor == Color(red, green, blue)) return;
  if (!connect()) return;

  uint8_t command[8] = {0x31, red, green, blue, 0, 0x00, 0x0F, 0};
  for (int i = 0; i < 7; i++) {
    command[7] += command[i];
  }

  zmsg_t* msg = zmsg_new();
  zframe_t* frame = zframe_new(m_identity.c_str(), m_identity.size());
  zmsg_append(msg, &frame);
  frame = zframe_new(command, 8);
  zmsg_append(msg, &frame);

  zmsg_send(&msg, m_socket);
  m_currentColor = Color(red, green, blue);
}
