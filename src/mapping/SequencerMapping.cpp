// Copyright 2019 Johan Lasperas
#include "mapping/SequencerMapping.h"

#include <cmath>

#include <boost/bind.hpp>

SequencerMapping::SequencerMapping(Launchpad* launchpad, Stripe* stripe)
    : Mapping(launchpad, stripe),
      m_active(false),
      m_bpm(120),
      m_syncDelayms(1000),
      m_tick(0),
      m_running(true),
      m_strobeOn(false),
      m_strobeState(0) {
  m_sequencerPage = new SequencerPage(launchpad);
  m_snakePage = new SnakePage(launchpad);
  m_currentPage = m_sequencerPage;

  m_bpm = 120;
  m_link = new ableton::Link(m_bpm);
  m_link->setTempoCallback([this] (const double bpm) {
      if (bpm < 180) {
        this->setBPM(bpm);
      }
    });

  m_link->enable(true);
  m_syncCounter = 0;
  m_syncBegin = bclock::now();
  m_syncEnd = bclock::now();
  m_sequencerThread = std::thread(&SequencerMapping::run, this);
}

SequencerMapping::~SequencerMapping() {
  m_running = false;
  m_sequencerThread.join();
  delete m_sequencerPage;
  delete m_snakePage;
  delete m_link;
}


void SequencerMapping::run() {
  int previousIndex = -1;

  while (m_running) {
    if (m_active) {
      ableton::Link::Timeline timeline = m_link->captureAudioTimeline();
      std::chrono::microseconds micros = m_link->clock().micros();

      int colorQuantum = TICKS_PER_BEAT / m_sequencerPage->getSpeed();
      double beat = timeline.phaseAtTime(micros, colorQuantum);
      int colorIndex = fmod(beat, colorQuantum) * 32 / colorQuantum;

      if (colorIndex != previousIndex) {
        m_currentPage->setCurrent(colorIndex);
        previousIndex = colorIndex;
      }

      if (m_strobeOn) {
        if (m_strobeState == 0) {
          m_stripe->setColor(Color::White);
        } else if (m_strobeState == 3) {
          m_stripe->setColor(Color::Black);
        }
        m_strobeState++;
        if (m_strobeState == 6) {
          m_strobeState = 0;
        }
      } else {
        Color color = m_sequencerPage->getColor(colorIndex);
        m_stripe->setColor(color);
      }
      usleep(10000);
    } else {
      usleep(100000);
    }
  }
}

void SequencerMapping::noteOn(int channel, int note) {
  if (note == NOTE_PAGE) {
    if (m_currentPage == m_sequencerPage) {
      m_currentPage = m_snakePage;
    } else {
      m_currentPage = m_sequencerPage;
    }
    refresh();
  } else if (note == NOTE_SPEED_DOWN) {
    m_sequencerPage->speedDown();
    refresh();
  } else if (note == NOTE_SPEED_UP) {
    m_sequencerPage->speedUp();
    refresh();
  } else if (note == NOTE_LIGHT_INTENSITY) {
    double intensity = m_stripe->getIntensity() + 0.25;
    if (intensity > 1.0) intensity -= 1;
    m_stripe->setIntensity(intensity);
    m_launchpad->setLed(NOTE_LIGHT_INTENSITY,
                     Color(255 * intensity, 255 * intensity,  255 * intensity));
  } else if (note == NOTE_DUMMY3) {
  } else if (note == NOTE_DECK) {
    m_sequencerPage->switchDeck();
    refresh();
  } else if (note == NOTE_STROBE) {
    m_strobeOn = true;
    m_launchpad->setLed(note, Color::White);
  } else {
    if (!m_currentPage->noteOn(note))
      pageClosed();
  }
}

void SequencerMapping::noteOff(int channel, int note) {
  if (note == NOTE_STROBE) {
    m_strobeOn = false;
    m_launchpad->setLed(note, Color::Grey);
  }
}


void SequencerMapping::setColors(const std::vector<Color>& colors) {
  int size = colors.size();
  for (int i = 0; i < 8; i++) {
    m_sequencerPage->setColor(0, 4*i, colors[i%size]);
    m_sequencerPage->setColor(0, 4*i+1, colors[i%size]);
    m_sequencerPage->setColor(0, 4*i+2, colors[i%size]);
    m_sequencerPage->setColor(0, 4*i+3, colors[i%size]);
  }
}

void SequencerMapping::start() {
  refresh();
  m_active = true;
}

void SequencerMapping::stop() {
  m_active = false;
}

/**
 *  Calling sync() multiple times on successive beats allows to resync Ableton Link on this device
 *  Currently not used, bu kept as is
 */
void SequencerMapping::sync() {
  int colorQuantum = TICKS_PER_BEAT / m_sequencerPage->getSpeed();

  btime syncCurrent = bclock::now();
  bduration elapsed = syncCurrent - m_syncEnd;

  // Sync hasn't been pressed in a while
  if (elapsed.count() / 1000000 > m_syncDelayms) {
    m_syncCounter = 0;
    m_syncBegin = syncCurrent;
  } else {
    m_syncCounter++;
  }

  m_syncEnd = syncCurrent;

  // Update BPM
  if (m_syncCounter > 3) {
    elapsed = m_syncEnd - m_syncBegin;
    m_bpm = 60*1e9/elapsed.count()*m_syncCounter;
    std::cout << m_bpm << std::endl;

    ableton::Link::Timeline timeline = m_link->captureAudioTimeline();
    timeline.setTempo(m_bpm, m_link->clock().micros());
    m_link->commitAudioTimeline(timeline);
  }
}

void SequencerMapping::pageClosed() {
  m_currentPage = m_sequencerPage;
  refresh();
}

void SequencerMapping::refresh() {
    m_currentPage->refresh();
    m_launchpad->flashLed(NOTE_SPEED_DOWN, Color::Grey);
    m_launchpad->flashLed(NOTE_SPEED_UP, Color::Grey);

    double intensity = m_stripe->getIntensity();
    m_launchpad->setLed(NOTE_LIGHT_INTENSITY,
                     Color(255 * intensity, 255 * intensity, 255 * intensity));
    m_launchpad->setLed(NOTE_STROBE, Color::Grey);
    m_launchpad->setLed(NOTE_DECK, m_sequencerPage->getDeck() ? Color(0, 255, 0).dim()
                                                           : Color(0, 0, 255).dim());
    m_launchpad->pulseLed(NOTE_MAPPING, 5);
}

