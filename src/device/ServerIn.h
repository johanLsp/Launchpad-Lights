// Copyright 2019 Johan Lasperas
#ifndef SRC_DEVICE_SERVERIN_H_
#define SRC_DEVICE_SERVERIN_H_

#include <czmq.h>
#include "device/DeviceIn.h"

class ServerIn : public DeviceIn {
 public:
    ServerIn();
    ~ServerIn();

    void receive();
    void run();
    void stop();

 private:
    bool m_running;
    zsock_t* m_server;
};

#endif  // SRC_DEVICE_SERVERIN_H_
