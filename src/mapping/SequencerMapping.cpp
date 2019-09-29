#include "mapping/SequencerMapping.h"

SequencerMapping::SequencerMapping(LaunchpadOut* output, Stripe* stripe)
:output(output), stripe(stripe) {
    sequencerPage = new SequencerPage(output);
    snakePage = new SnakePage(output);
    currentPage = sequencerPage;

    bpm = 120;
    link = new ableton::Link(bpm);
    link->setTempoCallback([this] (const double bpm) {
            if (bpm < 180) {
                this->setBPM(bpm);
            }
        });

    link->enable(true);

    syncCounter = 0;
    syncBegin = bclock::now();
    syncEnd = bclock::now();

    sequencerThread = std::thread(&SequencerMapping::run, this);
}

void SequencerMapping::run() {
    int previousIndex = -1;

    while (running) {
        if (active) {
            ableton::Link::Timeline timeline = link->captureAudioTimeline();
            std::chrono::microseconds micros = link->clock().micros();

            int colorQuantum = TICKS_PER_BEAT / sequencerPage->getSpeed();

            double beat = timeline.phaseAtTime(micros, colorQuantum);

            int colorIndex = fmod(beat, colorQuantum) * 32 / colorQuantum;

            if (colorIndex != previousIndex) {
                currentPage->setCurrent(colorIndex);
                previousIndex = colorIndex;
            }

            if (strobeOn) {
                if (strobeState == 0) {
                    stripe->setColor(Color::White);
                } else if (strobeState == 3) {
                    stripe->setColor(Color::Black);
                }
                strobeState++;
                if (strobeState == 6) {
                    strobeState = 0;
                }
            } else {
                Color color = sequencerPage->getColor(colorIndex);
                stripe->setColor(color);
            }

            usleep(10000);
        } else {
            usleep(100000);
        }
    }
}

void SequencerMapping::noteOn(int channel, int note) {
    if (note == NOTE_PAGE) {
        if (currentPage == sequencerPage) {
            currentPage = snakePage;
        } else {
            currentPage = sequencerPage;
        }
        refresh();
    } else if (note == NOTE_SPEED_DOWN) {
        sequencerPage->speedDown();
        refresh();
    } else if (note == NOTE_SPEED_UP) {
        sequencerPage->speedUp();
        refresh();
    } else if (note == NOTE_LIGHT_INTENSITY) {
        double intensity = stripe->getIntensity() + 0.25;
        if (intensity > 1.0) intensity -= 1;
        stripe->setIntensity(intensity);
        output->setLed(NOTE_LIGHT_INTENSITY, Color(255 * intensity,
                                                255 * intensity,
                                                255 * intensity));
    } else if (note == NOTE_DUMMY3) {
    } else if (note == NOTE_DECK) {
        sequencerPage->switchDeck();
        refresh();
    } else if (note == NOTE_STROBE) {
        strobeOn = true;
        output->setLed(note, Color::White);
    } else {
        if (!currentPage->noteOn(note))
            pageClosed();
    }
}

void SequencerMapping::noteOff(int channel, int note) {
    if (note == NOTE_STROBE) {
        strobeOn = false;
        output->setLed(note, Color::Grey);
    }
}


void SequencerMapping::setColors(std::vector<Color>& colors) {
    int size = colors.size();
    
    std::cout << (int)colors[0].red << ", " << (int)colors[0].green << ", " << (int)colors[0].blue << std::endl;
    for (int i = 0; i < 8; i++) {
        sequencerPage->setColor(0, 4*i, colors[i%size]);
        sequencerPage->setColor(0, 4*i+1, colors[i%size]);
        sequencerPage->setColor(0, 4*i+2, colors[i%size]);
        sequencerPage->setColor(0, 4*i+3, colors[i%size]);
    }
}

void SequencerMapping::start() {
    refresh();
    active = true;
}

void SequencerMapping::stop() {
    active = false;
}

void SequencerMapping::setBPM(double bpm) {
        this->bpm = bpm;
}

/**
 *  Calling sync() multiple times on successive beats allows to resync Ableton Link on this device
 *  Currently not used, bu kept as is
 */
void SequencerMapping::sync() {
    int colorQuantum = TICKS_PER_BEAT/sequencerPage->getSpeed();

    btime syncCurrent = bclock::now();
    bduration elapsed = syncCurrent - syncEnd;

    // Sync hasn't been pressed in a while
    if (elapsed.count() / 1000000 > syncDelayms) {
        syncCounter = 0;
        syncBegin = syncCurrent;
    } else {
        syncCounter++;
    }

    syncEnd = syncCurrent;

    // Update BPM
    if (syncCounter > 3) {
        elapsed = syncEnd - syncBegin;
        bpm = 60*1e9/elapsed.count()*syncCounter;
        std::cout << bpm << std::endl;

        ableton::Link::Timeline timeline = link->captureAudioTimeline();
        timeline.setTempo(bpm, link->clock().micros());
        link->commitAudioTimeline(timeline);
    }
}

void SequencerMapping::pageClosed() {
    currentPage = sequencerPage;
    refresh();
}


void SequencerMapping::refresh() {
    currentPage->refresh();
    output->flashLed(NOTE_SPEED_DOWN, Color::Grey);
    output->flashLed(NOTE_SPEED_UP, Color::Grey);

    double intensity = stripe->getIntensity();
    output->setLed(NOTE_LIGHT_INTENSITY, Color(255 * intensity,
                                            255 * intensity,
                                            255 * intensity));
    output->setLed(NOTE_STROBE, Color::Grey);
    output->setLed(NOTE_DECK, sequencerPage->getDeck() ? Color(0, 255, 0).dim()
                                                       : Color(0, 0, 255).dim());
    output->pulseLed(NOTE_MAPPING, 5);
}

SequencerMapping::~SequencerMapping() {
    running = false;
    sequencerThread.join();
}