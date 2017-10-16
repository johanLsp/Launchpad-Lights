#ifndef SEQUENCER_PAGE_H
#define SEQUENCER_PAGE_H

#define TICKS_PER_BEAT 16

#include "Page.h"
#include <iostream>

class SequencerPage : public Page
{
	public:
		SequencerPage(LaunchpadOut* output);
		~SequencerPage();
		
		void refresh();
		bool noteOn(int note);
		
		void setColor(int channel, int position);
		void setStrobe(int channel, int position);
		void speedUp(int channel);
        void speedDown(int channel);
		void switchDeck();
		
		// Getters
		Color getColor(int index) {return colors[currentDeck][index];};
		bool getStrobe(int index) {return strobe[currentDeck][index];};
		int getColorSpeed() {return speed[2*currentDeck];};
		int getStrobeSpeed() {return speed[2*currentDeck+1];};
		int getDeck() {return currentDeck;};
		
		
	private:
		LaunchpadOut* output;
		Color colors[2][16];
		bool strobe[2][16] = {true, true, true, true};
		int speed[4] = {4, 4, 4, 4};
		int currentDeck = 0;
	
	
};
#endif // SEQUENCER_PAGE_H
