// Copyright 2019 Johan Lasperas
#include <csignal>

#include "device/Launchpad.h"
#include "lights/LightGroup.h"
#include "lights/LedPwm.h"
#include "lights/LedRemote.h"
#include "lights/LightBluetooth.h"
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

enum Connected {None, Local, Remote, Color};

int main(int argc, char** argv) {
  LedPwm ledPwm;
  LedRemote ledRemote;
  LightBluetooth lightBle;
  LightGroup group;
  group.addLight(&ledRemote);
  group.addLight(&ledPwm);
  group.addLight(&lightBle);
  group.setColor(Color::White);

  MidiLocal midiLocal;
  Launchpad launchpadLocal(&midiLocal);

  MidiServer midiServer;
  Launchpad launchpadServer(&midiServer);
  midiServer.start();

  ColorServer colorServer;
  Launchpad launchpadColor(&colorServer);

  DirectMapping direct(&launchpadColor, &group);
  SequencerMapping sequencer(&launchpadColor, &group);

  launchpadLocal.addMapping(&direct);
  launchpadServer.addMapping(&direct);
  launchpadColor.addMapping(&direct);

  launchpadLocal.addMapping(&sequencer);
  launchpadServer.addMapping(&sequencer);
  launchpadColor.addMapping(&sequencer);

  Connected connected = None;

  signal(SIGINT, signalHandler);
  while (!done) {
    if (launchpadLocal.isConnected()) {
      if (connected != Local) {
        // Stop the two servers that have lower priority.
        colorServer.stop();
        midiServer.stop();
        direct.setLaunchpad(&launchpadLocal);
        sequencer.setLaunchpad(&launchpadLocal);
        connected = Local;
        std::cout << "Local Launchpad connected" << std::endl;
      }
    } else if (launchpadServer.isConnected()) {
      if (connected != Remote) {
        colorServer.stop();
        midiServer.start();
        direct.setLaunchpad(&launchpadServer);
        sequencer.setLaunchpad(&launchpadServer);
        connected = Remote;
        std::cout << "Remote Launchpad connected" << std::endl;
      }
    } else {
      if (connected != Color) {
        colorServer.start();
        midiServer.start();
        direct.setLaunchpad(&launchpadColor);
        sequencer.setLaunchpad(&launchpadColor);
        connected = Color;
        std::cout << "Color server connected" << std::endl;
      }
    }
    sleep(1);
  }
  midiServer.stop();
  colorServer.stop();

  return 0;
}
