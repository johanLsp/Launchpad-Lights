// Copyright 2019 Johan Lasperas
#ifndef SRC_MAPPING_PAGE_COLORPAGE_H_
#define SRC_MAPPING_PAGE_COLORPAGE_H_

#include "mapping/page/Page.h"

class ColorPage : public Page {
 public:
    void refresh() override;
    bool noteOn(int note) override;

    Color getColor() { return m_color; }

 private:
  Color m_color;
};

#endif  // SRC_MAPPING_PAGE_COLORPAGE_H_
