// Copyright 2019 Johan Lasperas
#include "transport/MidiServer.h"

#include <string>
#include <vector>

MidiServer::MidiServer()
    : m_running(false) {
  m_subscriber = zsock_new_sub(">tcp://127.0.0.1:7060", "MidiIn");
  m_publisher = zsock_new_pub(">tcp://127.0.0.1:7061");
  zsys_handler_set(NULL);
  m_connected = true;
}

void MidiServer::run() {
zactor_t* proxy = zactor_new(zproxy, NULL);
  zstr_sendx(proxy, "FRONTEND", "XSUB", "@tcp://*:7061", NULL);
  zsock_wait(proxy);
  zstr_sendx(proxy, "BACKEND", "XPUB", "@tcp://*:7060", NULL);
  zsock_wait(proxy);

  zpoller_t* poller = zpoller_new(m_subscriber, NULL);
  m_running = true;
  while (m_running) {
    void* poll = zpoller_wait(poller, 1000);
    if (zpoller_terminated(poller)) {
      m_running = false;
      break;
    }
    if (poll != nullptr) {
      zsock_t* reader = reinterpret_cast<zsock_t*>(poll);
      zframe_t* frame = zframe_recv(reader);
      // Strip the topic
      // Deal with mesage containing null characters.
      unsigned char* data = zframe_data(frame) + strlen("MidiIn ");
      int length = zframe_size(frame) - strlen("MidiIn ");
      ustring message_str(data, length);
      receive(message_str);
      zframe_destroy(&frame);
    }
  }
  zpoller_destroy(&poller);
  zactor_destroy(&proxy);
}

void MidiServer::send(const ustring& message) {
  std::string data = std::string("MidiOut ")
    + std::string(reinterpret_cast<const char*>(message.c_str()), message.size());
  zframe_t* frame = zframe_new(data.c_str(), data.size());
  zframe_send(&frame, m_publisher, 0);
  zframe_destroy(&frame);
}

void MidiServer::start() {
  m_thread = new std::thread(&MidiServer::run, this);
}

void MidiServer::stop() {
  m_running = false;
  m_thread->join();
}

MidiServer::~MidiServer() {
  zsock_destroy(&m_subscriber);
  zsock_destroy(&m_publisher);
  delete m_thread;
}
