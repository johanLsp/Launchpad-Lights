#include "SequencerMapping.h"



SequencerMapping::SequencerMapping(LaunchpadOut* output, Stripe* stripe)
:output(output), stripe(stripe)
{
	sequencerPage = new SequencerPage(output);
	colorPage = new ColorPage(output);
	currentPage = sequencerPage;
	
	bpm = 120;
	syncCounter = 0;
	syncBegin = bclock::now();
	syncEnd = bclock::now();
	

	sequencerThread = std::thread(&SequencerMapping::run, this);
}


void SequencerMapping::run()
{
	while(running)
	{   
	    if(active)
	    {
			if(strobeOn)
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
	            //std::cout << "Sleep : " << timeToSleepUs *ticksPerSeconds/1e6 << std::endl;
	        }
	    }
	    else
	    {
	        usleep(100000);
	    }
	}

}



void SequencerMapping::noteOn(int channel, int note)
{
	if(note == NOTE_COLOR)
	{
		if(currentPage != colorPage)
		{
			currentPage = colorPage;
		}
		else
		{
			currentPage = sequencerPage;
		}
		currentPage->refresh();
	}
	else if(note == NOTE_BPM)
	{
	    sync();
	}
	else if(note == NOTE_DUMMY1 || note == NOTE_DUMMY2 || note == NOTE_DUMMY3)
	{
	    sync();
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

void SequencerMapping::sync()
{
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
	}
}

void SequencerMapping::pageClosed()
{
	currentPage = sequencerPage;
	refresh();
}




void SequencerMapping::refresh()
{
	currentPage->refresh();
	output->setLed(NOTE_COLOR, colorPage->getColor());
	currentPage->setCurrentColor(colorPage->getColor());
	output->setLed(NOTE_DECK, sequencerPage->getDeck() ? Color(255,0,0) : Color(0,255,0));

}


SequencerMapping::~SequencerMapping()
{
	running = false;
	sequencerThread.join();
}
