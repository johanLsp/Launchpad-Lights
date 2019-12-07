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

  bool isConnected() override;
  void send(const ustring& message) override;
  void receiveCallback(cons ustring& message) { receive(message); }

 private:
  bool connect();
  int findLaunchpad(RtMidi* midi);

  RtMidiIn* m_input;
  RtMidiOut* m_output;
};

#endif  // SRC_TRANSPORT_MIDILOCAL_H_
