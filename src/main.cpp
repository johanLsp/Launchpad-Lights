// Copyright 2019 Johan Lasperas
#include <csignal>

#include "device/LaunchpadIn.h"
#include "device/ServerIn.h"
#include "mapping/DirectMapping.h"
#include "mapping/SequencerMapping.h"

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
  LaunchpadIn launchpadInput;
  LaunchpadOut launchpadOutput;
  ServerIn serverInput;

  DirectMapping direct(&launchpadOutput, &stripe);
  SequencerMapping sequencer(&launchpadOutput, &stripe);

  stripe.setColor(127, 127, 127);

  if (launchpadInput.isConnected()) {
    launchpadInput.addMapping(&direct);
    launchpadInput.addMapping(&sequencer);
    launchpadOutput.setAllLed(0);
    run();
  } else {
    std::cout << "Launchpad not connected, fallback to server mode"
              << std::endl;
    serverInput.addMapping(&sequencer);
    std::thread serverThread(&ServerIn::run, &serverInput);
    run();
    serverInput.stop();
    // TODO: Add a timeout to the ZeroMQ receive call so the
    // server thread does not block
    //serverThread.join();
  }
  return 0;
}
