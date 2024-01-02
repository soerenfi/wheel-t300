#include "wheel_event_reader.h"

#include <fcntl.h>
#include <linux/input.h>
#include <linux/joystick.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

WheelEventReader::WheelEventReader(const std::string& device_path)
    : device_path_(device_path), fd_(-1) {}

bool WheelEventReader::OpenDevice() {
  fd_ = open(device_path_.c_str(), O_RDWR);
  if (fd_ < 0) {
    return false;
  }
  device_stream_.open(device_path_, std::ios::in | std::ios::binary);
  return device_stream_.is_open();
}

void WheelEventReader::QueryDeviceCapabilities() {
  // Corrected initialization
  unsigned long evbit[(EV_MAX + 7) / 8] = {0};

  if (ioctl(fd_, EVIOCGBIT(0, EV_MAX), evbit) < 0) {
    std::cerr << "Error querying device capabilities" << std::endl;
    return;
  }

  if (evbit[EV_FF / 8] & (1 << (EV_FF % 8))) {
    std::cout << "Device supports force feedback" << std::endl;
    // Further queries for specific types of force feedback can be done here
  } else {
    std::cout << "Device does not support force feedback" << std::endl;
  }
}

bool WheelEventReader::ReadEvent(js_event& event) {
  if (!device_stream_.read(reinterpret_cast<char*>(&event), sizeof(js_event))) {
    return false;
  }
  return true;
}

// Other method implementations
