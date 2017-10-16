#include "SequencerPage.h"

SequencerPage::SequencerPage(LaunchpadOut* output)
:output(output)
{
}

SequencerPage::~SequencerPage()
{
}


void SequencerPage::refresh()
{
	output->setAllLed(0);
    

    for(int i = 0; i < 8; i++)
    {
        // 1st row
        output->setLed(i+1, 8, colors[0][i]);
        // 2nd row
        output->setLed(i+1, 7, colors[0][8+i]);
        
        output->setLed(i+1, 6, strobe[0][i] ? Color(255,255,255):Color(0,0,0));
        
        output->setLed(i+1, 5, strobe[0][8+i] ? Color(255,255,255):Color(0,0,0));
        
        output->setLed(i+1, 4, colors[1][i]);
        
        output->setLed(i+1, 3, colors[1][8+i]);
        
        output->setLed(i+1, 2, strobe[1][i] ? Color(255,255,255):Color(0,0,0));
        
        output->setLed(i+1, 1, strobe[1][8+i] ? Color(255,255,255):Color(0,0,0));
    }
}

bool SequencerPage::noteOn(int note)
{
			
    int chan =  3 - ((int)floor(1.0*(note-10)/20));
    
	if(note%10 == 9)
	{
		if( (note/10)%2 == 0)
			speedUp(chan);
		else
			speedDown(chan);
	}
	else
	{
		int index = (note-10)%20;
		if(index < 10) index += 7;
		else index -= 11;
		
		if(chan % 2 == 0)
		{
			setColor(chan/2, index);    
			output->setLed(note, currentColor);
		}
		else
		{
			setStrobe(chan/2, index);		
			output->setLed(note, strobe[chan/2][index] ? Color(255,255,255) : Color(0, 0, 0));
		}
		std::cout << "Channel " << chan << " | Index " << index << " (" << note << ")" << std::endl;
	}
		
    return true;
}

void SequencerPage::setColor(int channel, int index)
{
    colors[channel][index] = currentColor;

}

void SequencerPage::setStrobe(int channel, int index)
{
    strobe[channel][index] = !strobe[channel][index];

}

void SequencerPage::speedUp(int channel)
{
    if(speed[channel] == TICKS_PER_BEAT) return;
    speed[channel] *= 2;
    std::cout << "Speed channel " << channel << " : " << speed[channel] << std::endl;
}

void SequencerPage::speedDown(int channel)
{
    if(speed[channel] == 1) return;
    speed[channel] /= 2;
    std::cout << "Speed channel " << channel << " : " << speed[channel] << std::endl;
}

void SequencerPage::switchDeck()
{
	currentDeck = 1- currentDeck;
}
