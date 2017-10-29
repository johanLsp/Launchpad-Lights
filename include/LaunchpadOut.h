#ifndef LAUNCHPAD_OUT_H
#define LAUNCHPAD_OUT_H

#include "rtmidi/RtMidi.h"
#include <functional>
#include <cstdint>

class Color {
	
	public:
		Color():red(0),green(0),blue(0) {};
		Color(uint8_t red, uint8_t green, uint8_t blue)
		:red(red),green(green),blue(blue){};
		
		bool operator==(Color rhs)
		{
		  return rhs.red == red && rhs.green == green && rhs.blue == blue;
		}
		
		bool operator!=(Color rhs)
		{
		  return !(rhs==*this);
		}
		
		Color dim()
		{
			return Color(red/10, green/10, blue/10);
		}
	
	public:
		uint8_t red;
		uint8_t green;
		uint8_t blue;
};

class LaunchpadOut
{

        
	public:
	    LaunchpadOut();
	    ~LaunchpadOut();
	        		
		void setAllLed(int color);
		void setLed(uint8_t x, uint8_t y, uint8_t color);
		void setLed(uint8_t x, uint8_t y, Color color);
		void setLed(uint8_t note, Color color);
		void setLed(uint8_t note, uint8_t red, uint8_t green, uint8_t blue);
		void setLed(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue);
	
	private:
	    RtMidiOut *output;



};

#endif // LAUNCHPAD_OUT_H
