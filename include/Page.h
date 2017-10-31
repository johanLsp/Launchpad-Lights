#ifndef PAGE_H
#define PAGE_H
#include "LaunchpadOut.h"

#include <cmath>


class Page
{
	public:
		Page() {};
		Page(LaunchpadOut* output);
		virtual ~Page() = 0;
		
		virtual bool noteOn(int note) = 0;
		virtual void refresh() = 0;
		virtual void setCurrent(int index) = 0;
		
		void setCurrentColor(Color color);

		
	protected:
		LaunchpadOut* output;
		Color currentColor;
	
	
};

#endif // PAGE_H
