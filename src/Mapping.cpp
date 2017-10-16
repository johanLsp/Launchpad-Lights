#include "Mapping.h"

Mapping::Mapping()
{
}

Mapping::Mapping(LaunchpadOut* output, Stripe* stripe)
:output(output), stripe(stripe)
{
}

Mapping::~Mapping()
{
}
