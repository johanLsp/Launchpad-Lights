// Copyright 2019 Johan Lasperas
#ifndef CLIENT_MIDICLIENT_H_
#define CLIENT_MIDICLIENT_H_

#include <czmq.h>
#include <string>
#include <thread>

#include "RtMidi.h"

class MidiClient {
 public:
  MidiClient();
  ~MidiClient();

  void start();
  void stop();
  void forwardRemote(const std::string& message);
  void forwardLocal(const std::string& message);

 private:
  void attachServer();
  void attachMidi();
  void refresh();
  static void midiCallback(double timestamp,
                           std::vector<unsigned char>* message,
                           void* data);

  RtMidiIn* m_input;
  RtMidiOut* m_output;
  zsock_t* m_subscriber;
  zsock_t* m_publisher;
  zloop_t* m_reader;

  bool m_running;
  bool m_serverConnected;
  std::thread* m_server_thread;
  bool m_midiConnected;
  std::thread* m_midi_thread;
};

#endif  // CLIENT_MIDICLIENT_H_
