#include "SequencerMapping.h"



SequencerMapping::SequencerMapping(LaunchpadOut* output, Stripe* stripe)
:output(output), stripe(stripe)
{
	sequencerPage = new SequencerPage(output);
	colorPage = new ColorPage(output);
	currentPage = sequencerPage;

	bpm = 120;
	link = new ableton::Link(bpm);
	link->setTempoCallback([this] (const double bpm) {
if(bpm < 180){
	this->setBPM(bpm);
}});
	link->enable(true);
	std::cout << link->isEnabled() << std::endl;
	
	syncCounter = 0;
	syncBegin = bclock::now();
	syncEnd = bclock::now();
	

	sequencerThread = std::thread(&SequencerMapping::run, this);
}


void SequencerMapping::run()
{
	int previousIndex = -1;
	
	while(running)
	{   
	    if(active)
	    {
			ableton::Link::Timeline timeline = link->captureAudioTimeline();
			std::chrono::microseconds micros = link->clock().micros();
			
			int colorQuantum = TICKS_PER_BEAT/sequencerPage->getSpeed();

			//double beat = timeline.beatAtTime(micros, quantum);
			double beat = timeline.phaseAtTime(micros, colorQuantum);
							
			int colorIndex = fmod(beat, colorQuantum)*32/colorQuantum;
		
			Color color = sequencerPage->getColor(colorIndex);

			if(colorIndex != previousIndex)
			{
				sequencerPage->setCurrent(colorIndex);
				previousIndex = colorIndex;
			}
			
			if(strobeOn)
			{
				if(strobeState == 0)
				{
					stripe->setColor(255,255,255);
				}
				else if(strobeState == 3)
				{
					stripe->setColor(0,0,0);

				}
				strobeState++;
				if(strobeState == 6) strobeState = 0;
			}
			else
			{
				
				stripe->setColor(color.red, color.green, color.blue);
			}
		
		usleep(10000);
		

		//startTime = bclock::now() - boost::chrono::microseconds((int)(phaseMicros*1e3));

		
		
			/*if(strobeOn)
			{
				if(strobeState == 0)
				{
					stripe->setColor(255,255,255);
				}
				else if(strobeState == 1)
				{
					stripe->setColor(0,0,0);

				}
				strobeState++;
				if(strobeState == 2) strobeState = 0;
			}
			else
			{
				
				
				
				int colorSpeed = sequencerPage->getColorSpeed();
				int colorIndex = ((int)floor(1.0*tick*colorSpeed/TICKS_PER_BEAT)) % 16;
				
				int strobeSpeed = sequencerPage->getStrobeSpeed();
				int strobeIndex = ((int)floor(1.0*tick*strobeSpeed/TICKS_PER_BEAT)) % 16;
				
				if(sequencerPage->getStrobe(strobeIndex))
				{	
					Color color = sequencerPage->getColor(colorIndex);
					stripe->setColor(color.red, color.green, color.blue);  
				}
				else
				{
					stripe->setColor(0, 0, 0);
				}
			}
	        tick++;
	        
	        btime currentTime = bclock::now();
	        bduration elapsed = currentTime - startTime;
	        
	        double ticksPerSeconds = TICKS_PER_BEAT*1.0*bpm/60;
	        
	        double timeToSleepUs = tick/ticksPerSeconds*1e6 - elapsed.count()/1e3;
	        
	        if(timeToSleepUs > 0)
            {
	            usleep(timeToSleepUs);
	        }
	        std::cout << "Sleep : " << timeToSleepUs *ticksPerSeconds/1e6 << std::endl;
*/
	    }
	    else
	    {
	        usleep(100000);
	    }
	}

}



void SequencerMapping::noteOn(int channel, int note)
{
	if(note == NOTE_BPM)
	{
	    sync();
	}	
	else if(note == NOTE_SPEED_DOWN)
	{
	    sequencerPage->speedDown();
	    refresh();
	}
	else if(note == NOTE_SPEED_UP)
	{
	    sequencerPage->speedUp();
	    refresh();
	}
	else if(note == NOTE_DUMMY2 || note == NOTE_DUMMY3)
	{
	}
	else if(note == NOTE_DECK)
	{
		sequencerPage->switchDeck();
		refresh();
	}
	else if(note == NOTE_STROBE)
	{
		strobeOn = true;
		output->setLed(note, Color(255,255,255));
	}
	else
	{
		if(!currentPage->noteOn(note))
			pageClosed();
	}	
}

void SequencerMapping::noteOff(int channel, int note)
{
	if(note == NOTE_STROBE)
	{
		strobeOn = false;
		output->setLed(note, Color(0,0,0));

	}
}

void SequencerMapping::start()
{
    active = true;
	startTime = bclock::now();
	tick = 0;
	refresh();
}

void SequencerMapping::stop()
{
    active = false;
}

void SequencerMapping::setBPM(double bpm)
{
		this->bpm = bpm;		
}

void SequencerMapping::sync()
{
	
			
		int colorQuantum = TICKS_PER_BEAT/sequencerPage->getSpeed();
		
	std::cout << "Color : " << colorQuantum  << " | BPM : " << bpm << std::endl;
     btime syncCurrent = bclock::now();
     
	bduration elapsed = syncCurrent - syncEnd;
	
	// Sync hasn't been pressed in a while
	if(elapsed.count()/1000000 > syncDelayms)
	{
		syncCounter = 0;
		syncBegin = syncCurrent;
	}
	else 
	{
		syncCounter++;
	}
	
	syncEnd = syncCurrent;
	
	// Update BPM
	if(syncCounter > 3)
	{
		elapsed = syncEnd - syncBegin;
		bpm = 60*1e9/elapsed.count()*syncCounter;
		std::cout << bpm << std::endl;
		startTime = syncBegin;
		tick = 0;
		
		ableton::Link::Timeline timeline = link->captureAudioTimeline();
		timeline.setTempo(bpm, link->clock().micros());
		link->commitAudioTimeline(timeline);
	}
	
	/*
	
			this->bpm = bpm;
		
		ableton::Link::Timeline timeline = link->captureAppTimeline();
		std::chrono::microseconds micros = link->clock().micros();
		
		int quantum = TICKS_PER_BEAT/sequencerPage->getStrobeSpeed();
		
		double beat = timeline.beatAtTime(micros, quantum);
		double phase = timeline.phaseAtTime(micros, quantum);
		
		double phaseMicros = phase*60/bpm*1e6;
		
		startTime = bclock::now() - boost::chrono::microseconds((int)(phaseMicros*1e3));
		bduration d = bclock::now() - startTime;
		std::cout << d.count() << std::endl;
		
		std::cout << micros.count() << " - quantum : " << quantum << " - beat : " << beat << " - phase : " << phase << " (" << phaseMicros << ")"<< std::endl;


*/}

void SequencerMapping::pageClosed()
{
	currentPage = sequencerPage;
	refresh();
}




void SequencerMapping::refresh()
{
	currentPage->refresh();
	output->setLed(NOTE_SPEED_DOWN, Color(50, 50, 50));
	output->setLed(NOTE_SPEED_UP, Color(50, 50, 50));
	output->setLed(NOTE_STROBE, Color(255, 255, 255));
	currentPage->setCurrentColor(colorPage->getColor());
	output->setLed(NOTE_DECK, sequencerPage->getDeck() ? Color(255,0,0) : Color(0,255,0));

}


SequencerMapping::~SequencerMapping()
{
	running = false;
	sequencerThread.join();
}
