#ifndef SEQUENCER_MAPPING_H
#define SEQUENCER_MAPPING_H

#define NOTE_BPM 105
#define NOTE_COLOR 106
#define NOTE_DUMMY1 107
#define NOTE_DUMMY2 108
#define NOTE_DUMMY3 109
#define NOTE_STROBE 110
#define NOTE_DECK 111

#include "Mapping.h"
#include <boost/chrono.hpp>
#include <thread>
#include <cmath>

#include "ableton/Link.hpp"
#include <boost/bind.hpp>

#include "SequencerPage.h"
#include "ColorPage.h"

typedef boost::chrono::system_clock bclock;
typedef boost::chrono::system_clock::time_point btime;
typedef boost::chrono::system_clock::duration bduration;

class SequencerMapping : public Mapping
{	
	enum Mode {SequencerMode, ColorMode};
        
	public:
		SequencerMapping(LaunchpadOut* output, Stripe* stripe);
		~SequencerMapping();
		
		void noteOn(int channel, int note);
		void noteOff(int channel, int note);
		void start();
		void stop();
		
		
	private:
		void run();
		void sync();
		void setBPM(double bpm);
		void pageClosed();
        void refresh();

		
	private:
		LaunchpadOut* output;
		Stripe * stripe;
		
		// Mapping pages
		Page* currentPage;
		ColorPage* colorPage;
		SequencerPage* sequencerPage;
		
		bool active = false;

        // Sync variables
        ableton::Link* link;
		double bpm = 120;
		int syncDelayms = 1000;
		int syncCounter;
		btime syncBegin;
		btime syncEnd;
		
		btime startTime;
		
		int tick = 0;
		
		std::thread sequencerThread;
		bool running = true;
		bool strobeOn = false;
		int strobeState = 0;


};


#endif // SEQUENCER_MAPPING_H
