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
      char *message = zstr_recv(reader);
      ustring message_str(reinterpret_cast<unsigned char*>(message));
      receive(message_str);
      zstr_free(&message);
    }
  }
  zpoller_destroy(&poller);
  zactor_destroy(&proxy);
}

void MidiServer::send(const ustring& message) {
  const char* data = reinterpret_cast<const char*>(message.c_str());
  std::string frame = std::string("Midi ") + data;
  zstr_send(m_publisher, frame.c_str());
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
