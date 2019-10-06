// Copyright 2019 Johan Lasperas
#include "transport/MidiServer.h"

#include <string>
#include <vector>

MidiServer::MidiServer()
    : m_running(false) {
  m_subscriber = zsock_new_sub(">tcp://127.0.0.1:7060", "MidiIn");
  m_publisher = zsock_new_pub(">tcp://127.0.0.1:7061");

  // Setup a proxy to forward messages between the XSUB and XPUB sockets.
  m_proxy = zactor_new(zproxy, NULL);
  zstr_sendx(m_proxy, "FRONTEND", "XSUB", "@tcp://*:7061", NULL);
  zsock_wait(m_proxy);
  zstr_sendx(m_proxy, "BACKEND", "XPUB", "@tcp://*:7060", NULL);
  zsock_wait(m_proxy);

  // Disable the ZeroMQ signal handler to use the one defined in main.cpp
  zsys_handler_set(NULL);
  m_connected = true;
}

int MidiServer::receiveCallback(zloop_t* loop, zsock_t* reader, void* arg) {
  MidiServer* server = reinterpret_cast<MidiServer*>(arg);
  zframe_t* frame = zframe_recv(reader);
  // Strip the topic
  unsigned char* data = zframe_data(frame) + strlen("MidiIn ");
  int length = zframe_size(frame) - strlen("MidiIn ");
  ustring message_str(data, length);
  server->receive(message_str);
  zframe_destroy(&frame);
  return 0;
}

void MidiServer::send(const ustring& message) {
  std::string data = std::string("MidiOut ")
    + std::string(reinterpret_cast<const char*>(message.c_str()), message.size());
  zframe_t* frame = zframe_new(data.c_str(), data.size());
  zframe_send(&frame, m_publisher, 0);
  zframe_destroy(&frame);
}

void MidiServer::run() {
  m_reader = zloop_new();
  zloop_reader(m_reader, m_subscriber, MidiServer::receiveCallback, this);
  zloop_start(m_reader);
}

void MidiServer::start() {
  if (m_running) return;
  m_thread = new std::thread(&MidiServer::run, this);
}

void MidiServer::stop() {
  if (!m_running) return;
  m_running = false;
  zloop_reader_end(m_reader, m_subscriber);
  m_thread->join();
  delete m_thread;
}

MidiServer::~MidiServer() {
  stop();
  zloop_destroy(&m_reader);
  zactor_destroy(&m_proxy);
  zsock_destroy(&m_subscriber);
  zsock_destroy(&m_publisher);
}
