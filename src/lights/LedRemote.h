// Copyright 2019 Johan Lasperas
#ifndef SRC_LIGHTS_LEDREMOTE_H_
#define SRC_LIGHTS_LEDREMOTE_H_

#include <string>
#include <czmq.h>

#include "lights/Light.h"
#include "util/Color.h"

class LedRemote : public Light {
 public:
  LedRemote();
  ~LedRemote();
  void setColor(uint8_t red, uint8_t green, uint8_t blue) override;
 private:
  zsock_t* m_socket;
  std::string m_identity;
  Color m_currentColor;
};

#endif  // SRC_LIGHTS_LEDREMOTE_H_
