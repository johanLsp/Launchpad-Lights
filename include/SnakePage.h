#ifndef SNAKE_PAGE_H
#define SNAKE_PAGE_H

#include "Page.h"
#include <iostream>
#include "Color.h"
#include <deque>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

struct Cell {
	uint8_t x;
	uint8_t y;
	Color color;
	Cell()
	:x(1), y(1), color(Color::White) {};
	Cell(uint8_t x, uint8_t y, Color color)
	:x(x), y(y), color(color) {};
};

class SnakePage : public Page
{
	enum Direction {N, S, E, W};
	public:
		SnakePage(LaunchpadOut* output);
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
		bool gameOver = false;
	
	
};
#endif // SNAKE_PAGE_H
