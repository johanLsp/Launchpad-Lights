// Copyright 2019 Johan Lasperas
#ifndef CLIENT_MIDICLIENT_H_
#define CLIENT_MIDICLIENT_H_

#include <czmq.h>
#include <string>
#include <thread>

#include "rtmidi/RtMidi.h"

class MidiClient {
 public:
  MidiClient();
  ~MidiClient();

  void connect();
  void disconnect();
  void run();
  void start();
  void stop();
  void forwardRemote(const std::string& message);
  void forwardLocal(const std::string& message);

  bool isConnected();

 private:
  int setupLocalMidi();
  static void midiCallback(double timestamp, std::vector<unsigned char>* message, void* data);
  static int readerCallback(zloop_t* loop, zsock_t* reader, void* arg);

  bool m_running;
  bool m_connected;
  RtMidiIn* m_input;
  RtMidiOut* m_output;
  zsock_t* m_subscriber;
  zsock_t* m_publisher;
  zloop_t* m_reader;
  std::thread* m_thread;
};

#endif  // CLIENT_MIDICLIENT_H_
