// Copyright 2019 Johan Lasperas
#include "mapping/page/SnakePage.h"

#include <cmath>
#include <ctime>
#include <sstream>

SnakePage::SnakePage(Launchpad* launchpad)
    : Page(launchpad), m_score(0), m_gameOver(false), m_powerUp(false) {
  std::srand(std::time(0));
  m_body.push_back(Cell(6, 5, Color::Yellow));
  m_body.push_back(Cell(6, 4, Color::Green));
  m_body.push_back(Cell(6, 3, Color::Green));
  m_direction = N;
  newSeed();
}

void SnakePage::newSeed() {
  do {
    m_seed.x = 8.0 * rand() / RAND_MAX + 1;
    m_seed.y = 8.0 * rand() / RAND_MAX + 1;
    double randPower = 1.0 * rand() / RAND_MAX;
    if (randPower < 0.5 - exp(-1.0 * m_body.size() / 16)) {
      m_seed.color = Color::Blue;
    } else {
      m_seed.color = Color::Red;
    }
  } while (isColliding(m_seed, true));
}

bool SnakePage::isColliding(Cell cell, bool headIncluded) {
  bool colliding = false;
  int i = headIncluded ? 0 : 1;
  for (; i < m_body.size(); i++) {
    if (m_body[i].x == cell.x && m_body[i].y == cell.y)
      colliding = true;
  }
  return colliding;
}

void SnakePage::refresh() {
  m_launchpad->setAllLed(0);
  for (int i = 0; i < m_body.size(); i++) {
    m_launchpad->setLed(m_body[i].x, m_body[i].y, m_body[i].color);
  }
  m_launchpad->pulseLed(m_seed.x, m_seed.y, m_seed.color);
}

bool SnakePage::noteOn(int note) {
  if (note % 10 > 8) return true;
  if (note > 100)    return true;

  if (m_direction == N || m_direction == S) {
    if (note % 10 < 4) {
      m_direction = W;
    } else if (note % 10 > 5) {
      m_direction = E;
    }
  } else {
    if (note < 40) {
      m_direction = S;
    } else if (note > 50) {
      m_direction = N;
    }
  }
  return true;
}

// One iteration of the game loop
void SnakePage::setCurrent(int index) {
  if (m_gameOver) {
    endAnimation();
    return;
  }

  if (m_powerUp) {
    m_powerUpTimer--;
    if (m_powerUpTimer == 0) {
      m_powerUp = false;
      for (int i = 1; i < m_body.size(); i++) {
        m_body[i].color = Color::Green;
      }
    }
  }

  Cell lastCell = m_body.back();
  m_launchpad->setLed(lastCell.x, lastCell.y, 0);

  for (int i = m_body.size()-1; i > 0; i--) {
    m_body[i].x = m_body[i-1].x;
    m_body[i].y = m_body[i-1].y;
    m_launchpad->setLed(m_body[i].x, m_body[i].y, m_body[i].color);
  }
  moveCell(m_body[0], m_direction);
  m_launchpad->setLed(m_body[0].x, m_body[0].y, m_body[0].color);

  if (m_body[0].x == m_seed.x && m_body[0].y == m_seed.y) {
    if (m_seed.color == Color::Blue) {
      m_powerUp = true;
      m_powerUpTimer = 20;
      for (int i = 1; i < m_body.size(); i++) {
        m_body[i].color = Color::Blue;
      }
    } else {
      m_body.push_back(lastCell);
    }
    m_score++;
    newSeed();
    m_launchpad->pulseLed(m_seed.x, m_seed.y, m_seed.color);
  }
  if (isColliding(m_body[0], false)) {
    if (m_powerUp) {
      while (m_body.back().x != m_body[0].x | m_body.back().y != m_body[0].y) {
        m_body.pop_back();
      }
      m_powerUp = false;
      for (int i = 1; i < m_body.size(); i++) {
        m_body[i].color = Color::Green;
      }
      refresh();
    } else {
      m_gameOver = true;
      m_gameOverTimer = 20;
      m_launchpad->setLed(m_body[0].x, m_body[0].y, Color::Red);
    }
  }
}

void SnakePage::endAnimation() {
  if (m_body.size() > 3) {
    Cell lastCell = m_body.back();
    if (lastCell.x != m_body[0].x || lastCell.y != m_body[0].y) {
        m_launchpad->setLed(lastCell.x, lastCell.y, Color::Black);
    }
    m_body.pop_back();
    m_gameOverTimer = 21;
  } else if (m_gameOverTimer == 20) {
    std::stringstream gameoverStr;
    gameoverStr << m_score;
    m_launchpad->scrollText(gameoverStr.str());
  } else if (m_gameOverTimer == 0) {
    refresh();
    m_gameOver = false;
    m_score = 0;
  }
  m_gameOverTimer--;
}

void SnakePage::moveCell(Cell& cell, Direction direction) {
  switch (direction) {
    case N:
      cell.y++;
      break;
    case S:
      cell.y--;
      break;
    case E:
      cell.x++;
      break;
    case W:
      cell.x--;
      break;
  }
  if (cell.x == 0) cell.x = 8;
  if (cell.x == 9) cell.x = 1;
  if (cell.y  == 0) cell.y = 8;
  if (cell.y  == 9) cell.y = 1;
}
