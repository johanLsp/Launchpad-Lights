#ifndef INCLUDE_SNAKEPAGE_H_
#define INCLUDE_SNAKEPAGE_H_

#include <unistd.h>

#include <cstdlib>
#include <ctime>
#include <deque>
#include <iostream>
#include <sstream>

#include "Color.h"
#include "Page.h"

struct Cell {
    uint8_t x;
    uint8_t y;
    Color color;
    Cell()
    :x(1), y(1), color(Color::White) {}
    Cell(uint8_t x, uint8_t y, Color color)
    :x(x), y(y), color(color) {}
};

class SnakePage : public Page {
enum Direction {N, S, E, W};

 public:
    explicit SnakePage(LaunchpadOut* output);
    ~SnakePage();

    void refresh();
    bool noteOn(int note);
    void setCurrent(int index);



 private:
    void newSeed();
    bool isColliding(Cell cell, bool headIncluded);
    void endAnimation();



 private:
    LaunchpadOut* output;

    std::deque<Cell> body;
    Direction direction;
    Cell seed;
    int score = 0;
    bool gameOver = false;
    int gameOverTimer;
    bool powerUp = false;
    int powerUpTimer;


};
#endif  // INCLUDE_SNAKEPAGE_H_
