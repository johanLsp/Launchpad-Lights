#ifndef STRIPE_H
#define STRIPE_H

#include "cstdint"
#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include "Color.h"
#include <cstring>

class Stripe
{
	public:
		Stripe();
		~Stripe();
		
		void setColor(uint8_t red, uint8_t green, uint8_t blue);
		void setColor(Color color);

	private:
		bool wiringPiSupport;

		int redPin = 0;
        int greenPin = 9;
        int bluePin = 13;
};

#endif // STRIPE_H
