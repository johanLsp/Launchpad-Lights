// Copyright 2019 Johan Lasperas
#include <csignal>

#include "device/Launchpad.h"
#include "mapping/DirectMapping.h"
#include "mapping/SequencerMapping.h"
#include "transport/ColorServer.h"
#include "transport/MidiLocal.h"

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
  Stripe stripe;

  MidiLocal midiLocal;
  ColorServer colorServer;
  Launchpad launchpad(&midiLocal);

  DirectMapping direct(&launchpad, &stripe);
  SequencerMapping sequencer(&launchpad, &stripe);

  stripe.setColor(127, 127, 127);

  if (launchpad.isConnected()) {
    launchpad.addMapping(&direct);
    launchpad.addMapping(&sequencer);
    launchpad.setAllLed(0);
    run();
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
