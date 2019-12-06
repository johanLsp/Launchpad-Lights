// Copyright 2019 Johan Lasperas
#include <csignal>

#include "device/Launchpad.h"
#include "lights/LightGroup.h"
#include "lights/LedPwm.h"
#include "lights/LedRemote.h"
#include "mapping/DirectMapping.h"
#include "mapping/SequencerMapping.h"
#include "transport/ColorServer.h"
#include "transport/MidiLocal.h"
#include "transport/MidiServer.h"

bool done = false;
void signalHandler(int signum) {
  done = true;
}

void run() {
  signal(SIGINT, signalHandler);
  while (!done) {
    sleep(1);
  }
}

int main(int argc, char** argv) {
  LedPwm ledPwm;
  LedRemote ledRemote;
  LightGroup group;
  group.addLight(&ledRemote);
  group.addLight(&ledPwm);

  MidiLocal midi;
  ColorServer colorServer;
  Launchpad launchpad(&midi);

  DirectMapping direct(&launchpad, &group);
  SequencerMapping sequencer(&launchpad, &group);

  group.setColor(127, 127, 127);

  if (launchpad.isConnected()) {
    launchpad.addMapping(&direct);
    launchpad.addMapping(&sequencer);
    //launchpad.setAllLed(0);
    //midi.start();
    run();
    //midi.stop();
  } else {
    std::cout << "Launchpad not connected, fallback to server mode"
              << std::endl;
    launchpad = Launchpad(&colorServer);
    launchpad.addMapping(&sequencer);
    colorServer.start();
    // TODO: Add a timeout to the ZeroMQ receive call so the
    // server thread does not block
    //serverThread.join();
  }
  return 0;
}
