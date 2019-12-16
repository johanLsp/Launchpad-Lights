// Copyright 2019 Johan Lasperas
#include "lights/LightBluetooth.h"

#include <iomanip>
#include <iostream>

#include <boost/process/io.hpp>
#include <boost/process/search_path.hpp>

namespace bp = boost::process;

static const char kConnect[] = "connect FF:FF:CD:01:2E:25";
static const char kDisconnect[] = "disconnect FF:FF:CD:01:2E:25";
static const char kSetColor[] = "char-write-cmd 0x0007 56";
static const char kSetColorEnd[] = "00F0AA";

LightBluetooth::LightBluetooth()
    : m_connected(false) {
  connect();
  m_currentColor = Color::Black;
}

bool LightBluetooth::connect() {
  if (m_connected) return true;
  boost::filesystem::path gatttool = bp::search_path("gatttool");
  if (gatttool.empty()) {
    std::cerr << "Gatttool not found, Bluetooth light will not work." << std::endl;
    return false;
  }
  try {
    m_light_in.reset(new bp::ipstream());
    m_light_out.reset(new bp::opstream());
    m_light_child.reset(new bp::child(gatttool, "-I", bp::std_out > *m_light_in, bp::std_in < *m_light_out));
  } catch (const bp::process_error& e) {
    std::cerr << "Caught exception while calling Gatttool: " << e.what() << std::endl;
    return false;
  }
  if (!m_light_child) return false;
  *m_light_out << kConnect << std::endl;
  std::string line;
  while (m_light_child->running() && std::getline(*m_light_in, line)) {
    if (line.find("Connection successful") != std::string::npos) {
      m_connected = true;
      return m_connected;
    } else if (line.find("Failed") != std::string::npos ||
               line.find("Error") != std::string::npos) {
      std::cerr << "Bluetooth connection failed: " << line << std::endl;
      disconnect();
      m_connected = false;
      return m_connected;
    }
  }
  return false;
}

void LightBluetooth::disconnect() {
  *m_light_out << kDisconnect << std::endl;
}

LightBluetooth::~LightBluetooth() {
  if (m_connected) {
      *m_light_out << kDisconnect << std::endl;
  }
}

void LightBluetooth::setColor(uint8_t red, uint8_t green, uint8_t blue) {
  if (m_currentColor == Color(red, green, blue)) return;
  if (!connect()) {
    m_currentColor = Color(red, green, blue);
    return;
  }

  *m_light_out << kSetColor << std::setfill('0') << std::hex
             << std::setw(2) << static_cast<int>(red)
             << std::setw(2) << static_cast<int>(green)
             << std::setw(2) << static_cast<int>(blue)
             << kSetColorEnd << std::endl;
  std::string line;
  while (m_light_child->running() && std::getline(*m_light_in, line)) {
    if (line.find("Command Failed") != std::string::npos) {
      m_connected = false;
      return;
    }
    break;
  }
  m_currentColor = Color(red, green, blue);
}

