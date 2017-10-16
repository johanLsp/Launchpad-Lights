#include "LaunchpadOut.h"

LaunchpadOut::LaunchpadOut()
{
	output = new RtMidiOut();
	unsigned int nPorts = output->getPortCount();
	
	for(int i = 0; i < nPorts; i++)
	{
		std::string name = output->getPortName(i);		
		if(name.compare(0, 9, "Launchpad") == 0)
		{
			std::cout << "Output : " << name << std::endl;
			output->openPort(i);
		}
	}

	setAllLed(0);
}



void LaunchpadOut::setLed(uint8_t note, uint8_t red, uint8_t green, uint8_t blue)
{
	std::vector<unsigned char> message;
	message.push_back(240);
	message.push_back(0);
	message.push_back(32);
	message.push_back(41);
	message.push_back(2);
	message.push_back(24);
	message.push_back(11);
	message.push_back(note);
	message.push_back(red*63.0/255);
	message.push_back(green*63.0/255);
	message.push_back(blue*63.0/255);
	message.push_back(247);
	
	output->sendMessage(&message);
	
	output->sendMessage(&message);
}

void LaunchpadOut::setLed(uint8_t x, uint8_t y, uint8_t color)
{
	if(x < 1 || x > 8)
		return;
	if(y < 1 || y > 8) 
		return;
		
	std::vector<unsigned char> message;
	message.push_back(144);
	message.push_back(10*y+x);
	message.push_back(color);
	
	output->sendMessage(&message);
}

void LaunchpadOut::setLed(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	if(x < 1 || x > 8)
		return;
	if(y < 1 || y > 8) 
		return;

	setLed(10*y+x, red, green, blue);
}

void LaunchpadOut::setLed(uint8_t note, Color color)
{
	setLed(note, color.red, color.green, color.blue);
}

void LaunchpadOut::setLed(uint8_t x, uint8_t y, Color color)
{
	setLed(x, y, color.red, color.green, color.blue);
}


void LaunchpadOut::setAllLed(int color)
{
	std::vector<unsigned char> message;
	message.push_back(240);
	message.push_back(0);
	message.push_back(32);
	message.push_back(41);
	message.push_back(2);
	message.push_back(24);
	message.push_back(14);
	message.push_back(color);
	message.push_back(247);
	
	output->sendMessage(&message);

}


LaunchpadOut::~LaunchpadOut()
{
}
