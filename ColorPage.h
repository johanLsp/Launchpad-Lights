#ifndef COLOR_PAGE_H
#define COLOR_PAGE_H


#include "Page.h"


class ColorPage : public Page
{
	public:
		ColorPage(LaunchpadOut* output);
		~ColorPage();
		
		void refresh();
		bool noteOn(int note);
		
		// Getters
		Color getColor() {return currentColor;};
		
	private:
		LaunchpadOut* output;
		Color currentColor;
	
	
};

#endif // COLOR_PAGE_H
