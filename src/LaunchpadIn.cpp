#include "LaunchpadIn.h"


void recv_bind( double deltatime, std::vector< unsigned char > *message, void *userData )
{
	LaunchpadIn* launchpad = (LaunchpadIn*) userData;
	launchpad->receive(deltatime, message, userData);
}



LaunchpadIn::LaunchpadIn()
{
	input = new RtMidiIn();

	// Check available ports.
	unsigned int nPorts = input->getPortCount();

	for(int i = 0; i < nPorts; i++)
	{
		std::string name = input->getPortName(i);		
		if(name.compare(0, 9, "Launchpad") == 0)
		{
			input->openPort(i);
			
			input->setCallback(&recv_bind, this);
		}
	}
}


void LaunchpadIn::addMapping(Mapping* mapping)
{
	mappings.push_back(mapping);
	if(mappings.size() == 1)
	    mappings[0]->start();
}


void LaunchpadIn::receive(double deltatime, std::vector<unsigned char> *message, void *userData)
{
  unsigned int nBytes = message->size();
  /*
  for ( unsigned int i=0; i<nBytes; i++ )
    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltatime << std::endl;
  */  
    int channel = (int)message->at(0);
    int note    = (int)message->at(1);
	int command = (int)message->at(2);

    switch (command)
    {
    	case 127:
    	    if(note == 104)
    	    {
    	        changeMapping();
    	    }
    	    else
    	    {
    		    mappings[currentMapping]->noteOn(channel, note);
    		}
    		break;
    	case 0:
    		mappings[currentMapping]->noteOff(channel, note);
    		break;
    }
}


void LaunchpadIn::changeMapping()
{
    mappings[currentMapping]->stop();
    currentMapping++;
    if(currentMapping >= mappings.size())
        currentMapping = 0;
        
    mappings[currentMapping]->start();

}

LaunchpadIn::~LaunchpadIn()
{
}
