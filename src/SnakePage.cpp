#include "SnakePage.h"

SnakePage::SnakePage(LaunchpadOut* output)
:output(output)
{
	std::srand(std::time(0));

	body.push_back(Cell(6,5, Color::Yellow));
	body.push_back(Cell(6,4, Color::Green));
	body.push_back(Cell(6,3, Color::Green));

	direction = N;

	newSeed();
}

SnakePage::~SnakePage()
{
}


void SnakePage::newSeed()
{
	do
	{
		seed.x = 8.0*rand()/RAND_MAX +1;
		seed.y = 8.0*rand()/RAND_MAX +1;
		seed.color = Color::Red;
	} while (isColliding(seed, true));
}

bool SnakePage::isColliding(Cell cell, bool headIncluded)
{
	bool colliding = false;
	int i = headIncluded ? 0 : 1;
	for(; i < body.size(); i++)
	{
		if(body[i].x == cell.x && body[i].y == cell.y)
			colliding = true;
	}
	return colliding;
}

void SnakePage::refresh()
{
	output->setAllLed(0);

	for(int i = 0; i < body.size(); i++)
	{
		output->setLed(body[i].x, body[i].y, body[i].color);
	}

	output->pulseLed(seed.x, seed.y, seed.color);
}

bool SnakePage::noteOn(int note)
{

	if(note%10 > 8) return true;
	if(note > 100)  return true;

	if(direction == N || direction == S)
	{
		if(note%10 < 4) direction = W;
		else if(note%10 > 5) direction = E;

	}
	else
	{
		if(note < 40) direction = S;
		else if(note > 50) direction = N;
	}

    return true;
}


// One iteration of the game loop
void SnakePage::setCurrent(int index)
{

	if (gameOver)
	{
		endAnimation();
		return;
	}
	output->setLed(body.back().x, body.back().y, 0);
	int dx, dy;
	switch(direction)
	{
		case N:
			dx = 0; dy = 1;
			break;
		case S:
			dx = 0; dy = -1;
			break;
		case E:
			dx = 1; dy = 0;
			break;
		case W:
			dx = -1; dy = 0;
			break;
	}


	Cell lastCell = body.back();

	for(int i = body.size()-1; i > 0; i--)
	{
		body[i].x = body[i-1].x;
		body[i].y = body[i-1].y;
		output->setLed(body[i].x, body[i].y, body[i].color);
	}

	body[0].x += dx;
	body[0].y += dy;

	if(body[0].x < 1) body[0].x = 8;
	if(body[0].x > 8) body[0].x = 1;
	if(body[0].y < 1) body[0].y = 8;
	if(body[0].y > 8) body[0].y = 1;
	output->setLed(body[0].x, body[0].y, body[0].color);

	if(body[0].x == seed.x && body[0].y == seed.y)
	{
		body.push_back(lastCell);
		newSeed();
		output->pulseLed(seed.x, seed.y, seed.color);		
	}
	if(isColliding(body[0], false))
	{
		gameOver = true;

		output->setLed(body[0].x, body[0].y, Color::Red);
	}
}


void SnakePage::endAnimation()
{
	Cell lastCell = body.back();
	if(lastCell.x != body[0].x || lastCell.y != body[0].y)
		output->setLed(lastCell.x, lastCell.y, Color::Black);		
	body.pop_back();
	
	if(body.size() == 3)
		gameOver = false;
}