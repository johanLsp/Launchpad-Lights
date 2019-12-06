// Copyright 2019 Johan Lasperas
#include "lights/LedRemote.h"

#include <iostream>

LedRemote::LedRemote() {
  m_socket = zsock_new_stream(">tcp://192.168.0.50:5577");

  zmsg_t* msg = zmsg_recv(m_socket);
  for (int i = 0; i < zmsg_size(msg); i++) {
    zframe_t* frame = zmsg_pop(msg);
    byte* data = zframe_data(frame);
    m_identity = std::string(reinterpret_cast<const char*>(data),
                             zframe_size(frame));
  }
  m_currentColor = Color::Black;
}

LedRemote::~LedRemote() {
  zsock_destroy(&m_socket);
}

void LedRemote::setColor(uint8_t red, uint8_t green, uint8_t blue) {
  if (m_currentColor == Color(red, green, blue)) return;

  char command[8] = {0x31, red, green, blue, 0, 0x00, 0x0F, 0};
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
