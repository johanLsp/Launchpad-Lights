#include "SequencerMapping.h"
#include "DirectMapping.h"
#include "LaunchpadIn.h"

int main(int argc, char** argv)
{
	Stripe stripe;
	LaunchpadIn input;
	LaunchpadOut output;

	DirectMapping direct(&output, &stripe);
	SequencerMapping sequencer(&output, &stripe);

	input.addMapping(&direct);
	input.addMapping(&sequencer);

	stripe.setColor(127, 127, 127);

	while(1);
	output.setAllLed(0);
}
