#ifndef INCLUDE_LAUNCHPADIN_H_
#define INCLUDE_LAUNCHPADIN_H_

#include <cstdint>
#include <vector>
#include "rtmidi/RtMidi.h"
#include "Mapping.h"

typedef void(*RtMidiCallback)(double timeStamp,
                              std::vector< unsigned char > *message,
                              void *userData);

class LaunchpadIn {
 public:
    LaunchpadIn();
    ~LaunchpadIn();

    void addMapping(Mapping* mapping);

    void receive(double deltatime,
                 std::vector<unsigned char> *message,
                 void *userData);

    bool isConnected() {return connected;}

 private:
    void changeMapping();

 private:
    int currentMapping = 0;
    std::vector<Mapping*> mappings;
    RtMidiIn *input;
    bool connected;
};


#endif  // INCLUDE_LAUNCHPADIN_H_
