#include "DirectMapping.h"
#include "LaunchpadIn.h"
#include "ServerIn.h"
#include "SequencerMapping.h"
#include <csignal>

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
        serverThread.join();
    }
    return 0;
}
