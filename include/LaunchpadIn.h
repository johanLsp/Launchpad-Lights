#ifndef LAUNCHPAD_IN_H
#define LAUNCHPAD_IN_H

#include "rtmidi/RtMidi.h"
#include "Mapping.h"
#include <cstdint>

typedef void(* 	RtMidiCallback )(double timeStamp, std::vector< unsigned char > *message, void *userData);

class LaunchpadIn {

	public:
		LaunchpadIn();
		~LaunchpadIn();

		void addMapping(Mapping* mapping);
		
		void receive(double deltatime, std::vector<unsigned char> *message, void *userData);

	private:	
        void changeMapping();
		
	private:
	    int currentMapping = 0;
		std::vector<Mapping*> mappings;
		RtMidiIn *input;


};


#endif //LAUNCHPAD_IN_H
