#ifndef SEQUENCER_PAGE_H
#define SEQUENCER_PAGE_H

#define TICKS_PER_BEAT 16

#include "Page.h"
#include <iostream>
#include "Color.h"

class SequencerPage : public Page
{
	public:
		SequencerPage(LaunchpadOut* output);
		~SequencerPage();
		
		void refresh();
		bool noteOn(int note);
		
		void setColor(int channel, int position);
		void speedUp();
        void speedDown();
		void switchDeck();
		void setCurrent(int index);

		
		// Getters
		Color getColor(int index) {return colors[currentDeck][index];};
		int getSpeed() {return speed;};
		int getDeck() {return currentDeck;};
		Color getCurrentColor() {return palette[currentColorIdx];};
		
	private:
		static int getNote(int deck, int index);
		static int getDeck(int note);
		static int getIndex(int note);
		
		
	private:
		LaunchpadOut* output;
		Color colors[2][32];
		int speed = 1;
		int currentDeck = 0;
		
		int currentColorIdx = 0;
		Color palette[8];
	
	
};
#endif // SEQUENCER_PAGE_H
