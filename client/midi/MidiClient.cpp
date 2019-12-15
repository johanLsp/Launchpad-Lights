// Copyright 2019 Johan Lasperas
#include "MidiClient.h"

#include <iostream>

int main() {
  MidiClient client;
  client.start();
  while (!zsys_interrupted) {
    sleep(1);
  }
  client.stop();
}

MidiClient::MidiClient()
    : m_running(false), m_serverConnected(false), m_midiConnected(false),
      m_subscriber(nullptr), m_publisher(nullptr) {
  m_output = new RtMidiOut();
  m_input = new RtMidiIn();
  m_input->setCallback(&MidiClient::midiCallback, this);
}

void MidiClient::attachMidi() {
  while (m_running) {
    // Check whether the midi device is still connected.
    if (m_midiConnected) {
      bool connected = false;
      int nPorts = m_input->getPortCount();
      for (int i = 0; i < nPorts; i++) {
        std::string name = m_input->getPortName(i);
        if (name.compare(0, 9, "Launchpad") == 0) {
          connected = true;
        }
      }
      m_midiConnected = connected;
      if (!connected) {
        std::cout << "Launchpad disconnected" << std::endl;
        m_output->closePort();
        m_input->closePort();
      }
    // Try to connect a midi device.
    } else {
      // Connect the output.
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
      if (!connected) {
        m_midiConnected = false;
        continue;
      }
      // Connect the input.
      nPorts = m_input->getPortCount();
      for (int i = 0; i < nPorts; i++) {
        std::string name = m_input->getPortName(i);
        if (name.compare(0, 9, "Launchpad") == 0) {
          std::cout << "Input : " << name << std::endl;
          m_input->openPort(i);
          connected = true;
        }
      }
      if (connected) {
        refresh();
      }
      m_midiConnected = connected;
    }
    // Set the connection status to the server.
    forwardRemote(m_midiConnected ? "Connected" : "Disconnected");
    sleep(1);
  }
}

void MidiClient::attachServer() {
  // Set up the beacon detection to pick to Light server address
  zactor_t* beacon = zactor_new(zbeacon, NULL);
  zsock_send(beacon, "si", "CONFIGURE", 7062);
  char* hostname = zstr_recv(beacon);
  zsock_send(beacon, "sb", "SUBSCRIBE", "MidiServer", strlen("MidiServer"));
  // Create a poller to allow for non-blocking discovery.
  zpoller_t* beacon_poller = zpoller_new(beacon, NULL);

  while (m_running) {
    if (!m_serverConnected) {
      // Poll the beacon until we receive a message
      if (!zpoller_wait(beacon_poller, 200)) continue;

      zmsg_t* message = zmsg_recv(reinterpret_cast<zactor_t*>(beacon));
      size_t length = zmsg_size(message);
      if (length == 0) continue;

      // Get the server address
      char* ipaddress = zmsg_popstr(message);
      std::cout << "MidiServer found: " << ipaddress << std::endl;

      std::string sub_endpoint = std::string(">tcp://") + ipaddress + ":7060";
      std::string pub_endpoint = std::string(">tcp://") + ipaddress + ":7061";
      m_subscriber = zsock_new_sub(sub_endpoint.c_str(), "MidiOut");
      m_publisher = zsock_new_pub(pub_endpoint.c_str());

      m_serverConnected = true;
      // We somehow need to add some delay here otherwise the first message
      // might get lost. This seems to be a known issue.
      sleep(1);
      refresh();
    } else {
        // Set up a poller to process incoming messages and forward them to
        // MidiOut.
        zpoller_t* reader = zpoller_new(m_subscriber, NULL);
        while (m_running) {
          if (!zpoller_wait(reader, 200)) continue;

          // Forward the message to the Midi device.
          zframe_t* frame = zframe_recv(m_subscriber);
          // Strip the topic
          unsigned char* data = zframe_data(frame) + strlen("MidiOut ");
          int length = zframe_size(frame) - strlen("MidiOut ");
          std::string message_str(reinterpret_cast<char*>(data), length);
          forwardLocal(message_str);
          zframe_destroy(&frame);
        }
      m_serverConnected = false;
      zpoller_destroy(&reader);
      zsock_destroy(&m_subscriber);
      zsock_destroy(&m_publisher);
    }
  }
  zpoller_destroy(&beacon_poller);
  zactor_destroy(&beacon);
}

void MidiClient::midiCallback(double timestamp,
                              std::vector<unsigned char>* message,
                              void* data) {
  MidiClient* client = reinterpret_cast<MidiClient*>(data);
  std::string message_str(message->begin(), message->end());
  client->forwardRemote(message_str);
}

void MidiClient::forwardRemote(const std::string& message) {
  if (!m_serverConnected) return;
  std::string data = std::string("MidiIn ") + message;
  zframe_t* frame = zframe_new(data.c_str(), data.size());
  zframe_send(&frame, m_publisher, 0);
  zframe_destroy(&frame);
}

void MidiClient::forwardLocal(const std::string& message) {
  if (!m_midiConnected) return;
  std::vector<unsigned char> buffer(message.begin(), message.end());
  m_output->sendMessage(&buffer);
}

void MidiClient::refresh() {
  forwardRemote({0, 0, 105});
}

void MidiClient::start() {
  if (m_running) return;
  m_running = true;
  m_server_thread = new std::thread(&MidiClient::attachServer, this);
  m_midi_thread = new std::thread(&MidiClient::attachMidi, this);
}

void MidiClient::stop() {
  if (!m_running) return;
  m_running = false;
  m_server_thread->join();
  m_midi_thread->join();
  delete m_server_thread;
  delete m_midi_thread;
}

MidiClient::~MidiClient() {
  stop();
  delete m_input;
  delete m_output;
}
