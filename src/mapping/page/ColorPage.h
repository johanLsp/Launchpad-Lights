#ifndef INCLUDE_COLORPAGE_H_
#define INCLUDE_COLORPAGE_H_

#include "mapping/page/Page.h"

class ColorPage : public Page {
 public:
    explicit ColorPage(LaunchpadOut* output);
    ~ColorPage();

    void refresh();
    bool noteOn(int note);

    // Getters
    Color getColor() {return currentColor;}

 private:
    LaunchpadOut* output;
    Color currentColor;
};

#endif  // INCLUDE_COLORPAGE_H_
