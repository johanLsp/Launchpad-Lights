// Copyright 2019 Johan Lasperas
#include "SequencerPage.h"

SequencerPage::SequencerPage(Launchpad* launchpad)
: Page(launchpad) {
  m_palette[0] = Color::Red;
  m_palette[1] = Color::Green;
  m_palette[2] = Color::Blue;
  m_palette[3] = Color::Cyan;
  m_palette[4] = Color::Magenta;
  m_palette[5] = Color::Yellow;
  m_palette[6] = Color::Black;
  m_palette[7] = Color::White;
}


void SequencerPage::refresh() {
  m_launchpad->beginTransaction();

  for (int deck = 0; deck < 2; deck++) {
    for (int index = 0; index < 32; index++) {
      m_launchpad->setLed(getNote(deck, index), m_colors[deck][index].dim());
    }
  }
  for (int i = 0; i < 8; i++) {
    if (i == m_color_idx) {
      m_launchpad->setLed(9, i + 1, m_palette[i]);
    } else {
      m_launchpad->setLed(9, i + 1, m_palette[i].dim());
    }
  }
  m_launchpad->commitTransaction();
}

bool SequencerPage::noteOn(int note) {
  // Buttons on the right
  if (note%10 == 9) {
    m_launchpad->setLed(9, m_color_idx + 1, getCurrentColor().dim());
    m_color_idx = note / 10 - 1;
    m_launchpad->setLed(9, m_color_idx + 1, getCurrentColor());
  } else {
    int deck = getDeck(note);
    int index = getIndex(note);
    setColor(deck, index);
    m_launchpad->setLed(note, m_colors[deck][index].dim());
  }
  return true;
}

void SequencerPage::setColor(int deck, int index) {
  if (m_colors[deck][index] != getCurrentColor()) {
    m_colors[deck][index] = getCurrentColor();
  } else {
    m_colors[deck][index] = Color::Black;
  }
}

void SequencerPage::setColor(int deck, int index, Color color) {
  m_colors[deck][index] = color;
}

void SequencerPage::setCurrent(int index) {
  int previous = (index == 0 ? 31 : index-1);
  m_launchpad->setLed(getNote(m_currentDeck, previous),
                   m_colors[m_currentDeck][previous].dim());

  if (m_colors[m_currentDeck][index] == Color::Black) {
    m_launchpad->setLed(getNote(m_currentDeck, index), Color::Grey);
  } else {
    m_launchpad->setLed(getNote(m_currentDeck, index),
                     m_colors[m_currentDeck][index]);
  }
}

void SequencerPage::speedUp() {
  if (m_speed == TICKS_PER_BEAT) return;
  m_speed *= 2;
}

void SequencerPage::speedDown() {
  if (m_speed == 1) return;
  m_speed /= 2;
}

void SequencerPage::switchDeck() {
  m_currentDeck = 1 - m_currentDeck;
}

int SequencerPage::getNote(int deck, int index) {
  int x_idx = index % 8 + 1;
  int y_idx = 4 - index / 8 +(1 - deck) * 4;
  return 10 * y_idx + x_idx;
}

int SequencerPage::getDeck(int note) {
  return note > 50 ? 0 : 1;
}

int SequencerPage::getIndex(int note) {
  int x_idx = note % 10 - 1;
  int y_idx = ((note - 10) % 40) / 10;
  return x_idx + 8 * (3 - y_idx);
}
