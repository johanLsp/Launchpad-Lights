#include "Color.h"

const Color Color::Black = Color(0, 0, 0);
const Color Color::Red = Color(255, 0, 0);
const Color Color::Green = Color(0, 255, 0);
const Color Color::Blue = Color(0, 0, 255);
const Color Color::Cyan = Color(0, 255, 255);
const Color Color::Magenta = Color(255, 0, 255);
const Color Color::Yellow = Color(255, 255, 0);
const Color Color::White = Color(255, 255, 255);


bool Color::operator==(Color rhs)
{
  return rhs.red == red && rhs.green == green && rhs.blue == blue;
}

bool Color::operator!=(Color rhs)
{
  return !(*this==rhs);
}

Color Color::dim()
{
	return Color(red/10, green/10, blue/10);
}
