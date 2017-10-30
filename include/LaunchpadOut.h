#ifndef LAUNCHPAD_OUT_H
#define LAUNCHPAD_OUT_H

#include "rtmidi/RtMidi.h"
#include <functional>
#include "Color.h"

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
