#include "SequencerPage.h"

SequencerPage::SequencerPage(LaunchpadOut* output)
:output(output)
{
	palette[0] = Color(255,0,0);
	palette[1] = Color(0,255,0);
	palette[2] = Color(0,0,255);
	palette[3] = Color(255,255,0);
	palette[4] = Color(255,0,255);
	palette[5] = Color(0,255,255);
	palette[6] = Color(255,0,0);
	palette[7] = Color(255,255,255);

	for(int i = 0; i < 32; i++)
	{
		colors[0][i] = Color(0, 0, 0);
		colors[1][i] = Color(0, 0, 0);
	}
}

SequencerPage::~SequencerPage()
{
}


void SequencerPage::refresh()
{
	// Should think of a smart refresh without clearing the whole launchpad to avoid flickering
	output->setAllLed(0);

	for(int deck = 0; deck < 2; deck++)
	{
		for(int index = 0; index < 32; index++)
		{

			output->setLed(getNote(deck, index), colors[deck][index].dim());
		}
	}
    
    for(int i = 0; i < 8; i++)
    {
		if(i == currentColorIdx)
			output->setLed(9, i+1, palette[i]);
		else
			output->setLed(9, i+1, palette[i].dim());
	}
}

bool SequencerPage::noteOn(int note)
{
	
    // Buttons on the right
	if(note%10 == 9)
	{
		output->setLed(9, currentColorIdx+1, getCurrentColor().dim());
		currentColorIdx = note/10 - 1;
		output->setLed(9, currentColorIdx+1, getCurrentColor());
	}
	else
	{
		int deck = getDeck(note);
		int index = getIndex(note);

		setColor(deck, index);
        output->setLed(note, colors[deck][index].dim());
		std::cout << "Channel " << deck << " | Index " << index << " (" << note << ")" << std::endl;

	}
		
    return true;
}

void SequencerPage::setColor(int deck, int index)
{
	if(colors[deck][index] != getCurrentColor())
	{
		colors[deck][index] = getCurrentColor();
	}
	else
	{
		colors[deck][index] = Color(0,0,0);
	}
}

void SequencerPage::setCurrent(int index)
{
	int previous = (index == 0 ? 31 : index-1);

	output->setLed(getNote(currentDeck, previous), colors[currentDeck][previous].dim());
	
	if(colors[currentDeck][index] == Color(0,0,0))
		output->setLed(getNote(currentDeck, index), Color(50,50,50));
	else
		output->setLed(getNote(currentDeck, index), colors[currentDeck][index]);

}

void SequencerPage::speedUp()
{
    if(speed == TICKS_PER_BEAT) return;
    speed *= 2;
    std::cout << "Speed : " << speed << std::endl;
}

void SequencerPage::speedDown()
{
    if(speed == 1) return;
    speed /= 2;
    std::cout << "Speed : " << speed << std::endl;
}

void SequencerPage::switchDeck()
{
	currentDeck = 1- currentDeck;
}

int SequencerPage::getNote(int deck, int index)
{
	int x_idx = index%8 + 1;
	int y_idx = 4 - index/8 +(1-deck)*4;
	return 10*y_idx + x_idx;
}

int SequencerPage::getDeck(int note)
{
	return note > 50 ? 0 : 1;
}

int SequencerPage::getIndex(int note)
{
	int x_idx = note%10 - 1;
	int y_idx = ((note-10)%40)/10;
	return x_idx + 8*(3-y_idx);
}
