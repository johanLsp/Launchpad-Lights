// Copyright 2019 Johan Lasperas
#ifndef SRC_MAPPING_SEQUENCERMAPPING_H_
#define SRC_MAPPING_SEQUENCERMAPPING_H_

#define NOTE_PAGE 105
#define NOTE_SPEED_DOWN 106
#define NOTE_SPEED_UP 107
#define NOTE_LIGHT_INTENSITY 108
#define NOTE_DUMMY3 109
#define NOTE_STROBE 110
#define NOTE_DECK 111

#include <vector>
#include <thread>
#include <boost/chrono.hpp>

#include "ableton/Link.hpp"
#include "lights/Light.h"
#include "mapping/Mapping.h"
#include "mapping/page/SequencerPage.h"
#include "mapping/page/SnakePage.h"

typedef boost::chrono::system_clock bclock;
typedef boost::chrono::system_clock::time_point btime;
typedef boost::chrono::system_clock::duration bduration;

class SequencerMapping : public Mapping {
  enum Mode {SequencerMode, ColorMode};

 public:
  SequencerMapping(Launchpad* launchpad, Light* light);
  ~SequencerMapping();

  void noteOn(int channel, int note) override;
  void noteOff(int channel, int note) override;
  void start() override;
  void stop() override;
  void setColors(const std::vector<Color>& colors) override;

 private:
    void run();
    void sync();
    void setBPM(double bpm) { m_bpm = bpm; }
    void pageClosed();
    void refresh();

    // Mapping pages
    Page* m_currentPage;
    SequencerPage* m_sequencerPage;
    SnakePage* m_snakePage;

    bool m_active;

    // Sync variables
    ableton::Link* m_link;
    double m_bpm;
    int m_syncDelayms;
    int m_syncCounter;
    btime m_syncBegin;
    btime m_syncEnd;

    int m_tick;

    std::thread m_sequencerThread;
    bool m_running;
    bool m_strobeOn;
    int m_strobeState;
};

#endif  // SRC_MAPPING_SEQUENCERMAPPING_H_
