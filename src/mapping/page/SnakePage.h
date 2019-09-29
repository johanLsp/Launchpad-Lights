// Copyright 2019 Johan Lasperas
#ifndef SRC_MAPPING_PAGE_SNAKEPAGE_H_
#define SRC_MAPPING_PAGE_SNAKEPAGE_H_


#include <cstdlib>
#include <deque>

#include "Color.h"
#include "mapping/page/Page.h"

class SnakePage : public Page {
 private:
  enum Direction {N, S, E, W};
  struct Cell {
    uint8_t x;
    uint8_t y;
    Color color;
    Cell() : x(1), y(1), color(Color::White) {}
    Cell(uint8_t x, uint8_t y, Color color)
        : x(x), y(y), color(color) {}
  };

 public:
    explicit SnakePage(LaunchpadOut* output);

    void refresh() override;
    bool noteOn(int note) override;
    void setCurrent(int index) override;

 private:
    void newSeed();
    bool isColliding(Cell cell, bool headIncluded);
    void endAnimation();

    static void moveCell(Cell& cell, Direction direction);

    std::deque<Cell> m_body;
    Direction m_direction;
    Cell m_seed;
    int m_score = 0;
    bool m_gameOver = false;
    int m_gameOverTimer;
    bool m_powerUp = false;
    int m_powerUpTimer;
};
#endif  // SRC_MAPPING_PAGE_SNAKEPAGE_H_
