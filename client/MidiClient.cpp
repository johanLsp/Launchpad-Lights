// Copyright 2019 Johan Lasperas
#include "MidiClient.h"

#include <iostream>


bool shouldExit = false;
void signalHandler(int signum) {
  shouldExit = true;
}

int main() {
  signal(SIGINT, signalHandler);
  MidiClient client;
  while (!shouldExit) {
    client.connect();
    while (!client.isConnected() && !shouldExit) {
      sleep(1);
      client.connect();
    }
    client.start();
    while (client.isConnected() && !shouldExit) {
      sleep(1);
    }
    client.stop();
    client.disconnect();
  }
}

void receiveCallback(double timestamp, std::vector<unsigned char>* message,
                     void* data) {
  MidiClient* client = reinterpret_cast<MidiClient*>(data);
  std::string message_str(message->begin(), message->end());
  client->forwardRemote(message_str);
}

MidiClient::MidiClient()
    : m_connected(false), m_running(false), m_thread(nullptr) {
  m_output = new RtMidiOut();
  m_input = new RtMidiIn();
  m_input->setCallback(&::receiveCallback, this);

  m_subscriber = zsock_new_sub(">tcp://127.0.0.1:7060", "MidiOut");
  m_publisher = zsock_new_pub(">tcp://127.0.0.1:7061");
  zsys_handler_set(NULL);
}

void MidiClient::connect() {
  unsigned int nPorts = m_output->getPortCount();

  bool connected = false;
  for (int i = 0; i < nPorts; i++) {
    std::string name = m_output->getPortName(i);
    if (name.compare(0, 9, "Launchpad") == 0) {
      std::cout << "Output : " << name << std::endl;
      m_output->openPort(i);
      connected = true;
    }
  }
  if (!connected) return;
  // Check available ports.
  nPorts = m_input->getPortCount();
  for (int i = 0; i < nPorts; i++) {
    std::string name = m_input->getPortName(i);
    if (name.compare(0, 9, "Launchpad") == 0) {
      std::cout << "Input : " << name << std::endl;
      m_input->openPort(i);
      connected = true;
    }
  }
  if (!connected) return;
  m_connected = true;
}

bool MidiClient::isConnected() {
  int nPorts = m_input->getPortCount();
  for (int i = 0; i < nPorts; i++) {
    std::string name = m_input->getPortName(i);
    if (name.compare(0, 9, "Launchpad") == 0) {
      return m_connected;
    }
  }
  return false;
}

void MidiClient::disconnect() {
  std::cout << "Disconnecting" << std::endl;
  m_output->closePort();
  m_input->closePort();
  m_connected = false;
}

void MidiClient::run() {
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
      unsigned char* data = zframe_data(frame) + strlen("MidiOut ");
      int length = zframe_size(frame) - strlen("MidiOut ");
      std::string message_str(reinterpret_cast<char*>(data), length);
      forwardLocal(message_str);
      zframe_destroy(&frame);
    }
  }
  zpoller_destroy(&poller);
}

void MidiClient::forwardRemote(const std::string& message) {
  std::string data = std::string("MidiIn ") + message;
  zframe_t* frame = zframe_new(data.c_str(), data.size());
  zframe_send(&frame, m_publisher, 0);
  zframe_destroy(&frame);
}

void MidiClient::forwardLocal(const std::string& message) {
  std::vector<unsigned char> buffer(message.begin(), message.end());
  m_output->sendMessage(&buffer);
}

void MidiClient::start() {
  m_thread = new std::thread(&MidiClient::run, this);
  forwardRemote({0, 0, -128});
}

void MidiClient::stop() {
  m_running = false;
  m_thread->join();
  delete m_thread;
  m_thread = nullptr;
}

MidiClient::~MidiClient() {
  zsock_destroy(&m_subscriber);
  zsock_destroy(&m_publisher);
  delete m_input;
  delete m_output;
  delete m_thread;
  m_thread = nullptr;
}
