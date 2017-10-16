#ifndef DIRECT_MAPPING_H
#define DIRECT_MAPPING_H

#include "Mapping.h"
#include <functional>
#include <cmath>

class DirectMapping : public Mapping
{
	public:
	    DirectMapping(LaunchpadOut* output, Stripe* stripe);
	    ~DirectMapping();
	    
		void noteOn(int channel, int note);
		void noteOff(int channel, int note);
		void start();
		void stop();
		
	private:
	    LaunchpadOut* output;
	    Stripe* stripe;
	    
	    int num_pressed = 0;


};


#endif // DIRECT_MAPPING_H
