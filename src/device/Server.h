// Copyright 2019 Johan Lasperas
#ifndef SRC_DEVICE_SERVER_H_
#define SRC_DEVICE_SERVER_H_

#include <czmq.h>
#include "device/Device.h"

class Server : public Device {
 public:
    Server();
    ~Server();

    void receive();
    void send(const ustring& message) override {};
    void run();
    void stop();

 private:
    bool m_running;
    zsock_t* m_server;
};

#endif  // SRC_DEVICE_SERVER_H_
