#include "Stripe.h"


Stripe::Stripe()
{
    if ((fopen ("/proc/cpuinfo", "r")) == NULL)
        wiringPiSupport = false;
    
    if(!wiringPiSupport) return;

    wiringPiSetup();
    softPwmCreate(redPin, 0, 100);
    softPwmCreate(bluePin, 0, 100);
    softPwmCreate(greenPin, 0, 100);   
}


Stripe::~Stripe()
{
}



void Stripe::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    if(!wiringPiSupport) return;

    softPwmWrite(redPin, 100.0/255*red);
    softPwmWrite(greenPin, 100.0/255*green);
    softPwmWrite(bluePin, 100.0/255*blue);
}


void Stripe::setColor(Color color)
{
	setColor(color.red, color.green, color.blue);
}
