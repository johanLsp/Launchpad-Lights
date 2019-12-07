// Copyright 2019 Johan Lasperas
#ifndef SRC_MAPPING_PAGE_PAGE_H
#define SRC_MAPPING_PAGE_PAGE_H

#include "device/Launchpad.h"

class Page {
 public:
  explicit Page(Launchpad* launchpad) : m_launchpad(launchpad) {}
  virtual bool noteOn(int note) = 0;
  virtual void refresh() = 0;
  virtual void setCurrent(int index) = 0;

  virtual void setLaunchpad(Launchpad* launchpad) {
    m_launchpad = launchpad;
  }

 protected:
  Launchpad* m_launchpad;
};

#endif  // SRC_MAPPING_PAGE_PAGE_H
