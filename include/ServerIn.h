#ifndef INCLUDE_SERVERIN_H_
#define INCLUDE_SERVERIN_H_

#include <cstdint>
#include <vector>
#include <czmq.h>
#include "Mapping.h"
#include "Color.h"
#include "json.hpp"

class ServerIn {
 public:
    ServerIn();
    ~ServerIn();

    void addMapping(Mapping* mapping);
    void receive();
    void run();
    void stop();

 private:
    void changeMapping();

 private:
    int currentMapping = 0;
    bool running;
    std::vector<Mapping*> mappings;
    zsock_t* server;
};


#endif  // INCLUDE_SERVERIN_H_
