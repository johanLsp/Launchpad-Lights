// Copyright 2019 Johan Lasperas
#ifndef SRC_MAPPING_PAGE_PAGE_H
#define SRC_MAPPING_PAGE_PAGE_H

#include "device/LaunchpadOut.h"

class Page {
 public:
  explicit Page(LaunchpadOut* output) : m_output(output) {}
  virtual bool noteOn(int note) = 0;
  virtual void refresh() = 0;
  virtual void setCurrent(int index) = 0;

 protected:
  LaunchpadOut* m_output;
};

#endif  // SRC_MAPPING_PAGE_PAGE_H
