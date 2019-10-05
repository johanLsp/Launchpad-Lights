// Copyright 2019 Johan Lasperas
#ifndef SRC_TRANSPORT_MIDISERVER_H_
#define SRC_TRANSPORT_MIDISERVER_H_

#include <thread>
#include <czmq.h>
#include "transport/Transport.h"

/// The MidiServer implement a bi-directional PUB/SUB pattern
/// to allow for both the client and server to asynchronously send/receive
/// Midi messages. The communication from Server to Client is under the topic
/// 'MidiOut' while the communication from Client to Server is under the topic
/// 'MidiIn'. An additional XPUB/XSUB proxy is used to allow for dynamic
/// discovery of the Server/Client.
class MidiServer : public Midi {
 public:
  MidiServer();
  ~MidiServer();

  void run();
  void start();
  void stop();
  void send(const ustring& message) override;

 private:
  bool m_running;
  zsock_t* m_subscriber;
  zsock_t* m_publisher;
  std::thread* m_thread;
};

#endif  // SRC_TRANSPORT_MIDISERVER_H_
