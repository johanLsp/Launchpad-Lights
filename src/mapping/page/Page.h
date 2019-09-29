#ifndef INCLUDE_PAGE_H_
#define INCLUDE_PAGE_H_
#include "device/LaunchpadOut.h"

#include <cmath>


class Page {
 public:
    Page() {}
    explicit Page(LaunchpadOut* output);
    virtual ~Page() = 0;
    virtual bool noteOn(int note) = 0;
    virtual void refresh() = 0;
    virtual void setCurrent(int index) = 0;

    void setCurrentColor(Color color);

 protected:
    LaunchpadOut* output;
    Color currentColor;
};

#endif  // INCLUDE_PAGE_H_
