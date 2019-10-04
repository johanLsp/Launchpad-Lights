// Copyright 2019 Johan Lasperas
#ifndef SRC_TRANSPORT_COLORSERVER_H_
#define SRC_TRANSPORT_COLORSERVER_H_

#include <czmq.h>
#include "transport/Transport.h"

class ColorServer : public Transport {
 public:
  ColorServer();
  ~ColorServer();

  void run();
  void start();
  void stop();

  void send(const ustring& message) override {}


 private:
  bool m_running;
  zsock_t* m_server;
};

#endif  // SRC_TRANSPORT_COLORSERVER_H_
