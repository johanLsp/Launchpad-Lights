// Copyright 2019 Johan Lasperas
#ifndef SRC_MAPPING_PAGE_SEQUENCERPAGE_H_
#define SRC_MAPPING_PAGE_SEQUENCERPAGE_H_

#define TICKS_PER_BEAT 16

#include "mapping/page/Page.h"
#include "util/Color.h"

class SequencerPage : public Page {
 public:
  explicit SequencerPage(Launchpad* launchpad);

  void refresh() override;
  bool noteOn(int note) override;
  void setCurrent(int index) override;

  void setColor(int deck, int index);
  void setColor(int deck, int index, Color color);
  void speedUp();
  void speedDown();
  void switchDeck();

  // Getters
  Color getColor(int index) {return m_colors[m_currentDeck][index];}
  int getSpeed() {return m_speed;}
  int getDeck() {return m_currentDeck;}
  Color getCurrentColor() {return m_palette[m_color_idx];}

 private:
  static int getNote(int deck, int index);
  static int getDeck(int note);
  static int getIndex(int note);

 private:
  Color m_palette[8];
  Color m_colors[2][32];
  int m_color_idx = 0;
  int m_speed = 2;
  int m_currentDeck = 0;
};

#endif  // SRC_MAPPING_PAGE_SEQUENCERPAGE_H_
