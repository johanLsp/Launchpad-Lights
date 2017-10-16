#ifndef MAPPING_H
#define MAPPING_H

#include "LaunchpadOut.h"
#include "Stripe.h"

class Mapping
{
	public:
	    Mapping();
	    Mapping(LaunchpadOut* output, Stripe* stripe);
	    ~Mapping(); 
		
		virtual void noteOn(int channel, int note) = 0;
		virtual void noteOff(int channel, int note) = 0;
		
		virtual void start() = 0;
		virtual void stop() = 0;
	    
	    
    private:
        LaunchpadOut* output;
        Stripe* stripe;
	   
	   
};

#endif // MAPPING_H
