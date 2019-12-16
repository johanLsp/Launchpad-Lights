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
  setupBeacon();

  // Disable the ZeroMQ signal handler to use the one defined in main.cpp
  //zsys_handler_set(NULL);
  m_connected = false;
}

void MidiServer::setupBeacon() {
  m_beacon = zactor_new(zbeacon, NULL);
  zsock_send(m_beacon, "si", "CONFIGURE", 7062);
  char *hostname = zstr_recv(m_beacon);
  std::cout << "Beacon set up on " << hostname << std::endl;
  zsock_send(m_beacon, "sbi", "PUBLISH", "MidiServer",
             strlen("MidiServer"), 1000);
}

int MidiServer::receiveCallback(zloop_t* loop, zsock_t* reader, void* arg) {
  MidiServer* server = reinterpret_cast<MidiServer*>(arg);
  zframe_t* frame = zframe_recv(reader);
  const char* data = reinterpret_cast<const char*>(zframe_data(frame));
  std::string message(data, zframe_size(frame));
  if (message.rfind("MidiIn ", 0) != 0) {
      std::cout << "Invalid message: " << message << std::endl;
      return -1;
  }
  server->receive(message.substr(strlen("MidiIn ")));
  zframe_destroy(&frame);
  return 0;
}

void MidiServer::receive(const std::string& message) {
  // Intercept connection status.
  if (message.find("Connected") != std::string::npos) {
    m_connected = true;
  } else if (message.find("Disconnected") != std::string::npos) {
    m_connected = false;
  } else {
    Transport::receive(message);
  }
}

void MidiServer::send(const std::string& message) {
  std::string data = std::string("MidiOut ") + message;
  zframe_t* frame = zframe_new(data.c_str(), data.size());
  zframe_send(&frame, m_publisher, 0);
  zframe_destroy(&frame);
}

void MidiServer::run() {
  m_running = true;
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
  zloop_destroy(&m_reader);
  //m_thread->join();
  delete m_thread;
}

MidiServer::~MidiServer() {
  stop();
  zactor_destroy(&m_beacon);
  zactor_destroy(&m_proxy);
  zsock_destroy(&m_subscriber);
  zsock_destroy(&m_publisher);
}
