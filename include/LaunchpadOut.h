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
		void flashLed(int note, Color color);

		void pulseLed(int x, int y, Color color);
		void pulseLed(int note, int color);
		
		void beginTransaction();
		void commitTransaction();


	
	private:
	    RtMidiOut *output;
	    bool transactional = false;
	    std::vector<unsigned char> message;
};

#endif // LAUNCHPAD_OUT_H
