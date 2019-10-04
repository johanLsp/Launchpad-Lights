// Copyright 2019 Johan Lasperas
#ifndef SRC_TRANSPORT_MIDILOCAL_H_
#define SRC_TRANSPORT_MIDILOCAL_H_

#include <vector>

#include "rtmidi/RtMidi.h"
#include "transport/Transport.h"


class MidiLocal : public Midi {
 public:
  MidiLocal();
  ~MidiLocal();

  void send(const ustring& message) override;
  void receiveCallback(const ustring& message) { receive(message); }

 private:
  RtMidiIn* m_input;
  RtMidiOut* m_output;
};

#endif  // SRC_TRANSPORT_MIDILOCAL_H_
