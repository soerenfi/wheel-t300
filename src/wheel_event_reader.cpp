
#include "wheel_event_reader.h"

#include <fcntl.h>
#include <linux/input.h>
#include <linux/input.h>  // for FF_STATUS
#include <linux/joystick.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#define BITS_TO_LONGS(x) \
  (((x) + 8 * sizeof(unsigned long) - 1) / (8 * sizeof(unsigned long)))

WheelEventReader::WheelEventReader(const std::string& device_path)
    : device_path_(device_path), fd_(-1) {}

bool WheelEventReader::OpenDevice() {
  fd_ = open(device_path_.c_str(), O_RDONLY | O_NONBLOCK);
  if (fd_ < 0) {
    return false;
  }
  return true;
  // device_stream_.open(device_path_, std::ios::in | std::ios::binary);
  // return device_stream_.is_open();
}

void WheelEventReader::QueryDeviceCapabilities() {
  // Corrected initialization

  char name[128];
  ioctl(fd_, JSIOCGNAME(sizeof(name)), name);
  std::cout << "Connected to joystick: " << name << std::endl;

  int number_of_axes = 0;
  ioctl(fd_, JSIOCGAXES, &number_of_axes);
  std::cout << "Number of axes: " << number_of_axes << std::endl;

  int number_of_buttons = 0;
  ioctl(fd_, JSIOCGBUTTONS, &number_of_buttons);
  std::cout << "Number of buttons: " << number_of_buttons << std::endl;

  unsigned long features[BITS_TO_LONGS(FF_CNT)];

  if (ioctl(fd_, EVIOCGBIT(EV_FF, sizeof(features)), features) < 0) {
    std::cerr << "Error querying device capabilities" << std::endl;
  }

  // if (ioctl(fd_, EVIOCGBIT(0, EV_MAX), evbit) < 0) {
  //   std::cerr << "Error querying device capabilities" << std::endl;
  //   return;
  // }

  // if (evbit[EV_FF / 8] & (1 << (EV_FF % 8))) {
  //   std::cout << "Device supports force feedback" << std::endl;
  //   // Further queries for specific types of force feedback can be done here
  // } else {
  //   std::cout << "Device does not support force feedback" << std::endl;
  // }
}

bool WheelEventReader::ReadEvent(js_event& event) {
  ssize_t bytes = read(fd_, &event, sizeof(js_event));
  if (bytes == -1) {
    // Handle read error
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // No data available, non-blocking mode
      return false;
    } else {
      // Handle other read errors
      std::cerr << "Error reading from device: " << strerror(errno)
                << std::endl;
      return false;
    }
  } else if (bytes == sizeof(js_event)) {
    return true;
  }
  return false;
}

bool WheelEventReader::HasForceFeedbackSupport() {
  int features[2] = {0};
  if (ioctl(fd_, EVIOCGBIT(EV_FF, sizeof(features)), features) < 0) {
    std::cerr << "Failed to query force feedback support: " << strerror(errno)
              << std::endl;
    return false;
  }
  return (features[0] & (1 << FF_RUMBLE)) != 0;
}

void WheelEventReader::QueryAxisRanges() {
  struct input_absinfo abs_info;
  for (int i = 0; i < ABS_CNT; ++i) {
    if (ioctl(fd_, EVIOCGABS(i), &abs_info) < 0) {
      std::cerr << "Failed to query axis range " << i << ": " << strerror(errno)
                << std::endl;
    } else {
      std::cout << "Axis " << i << " range: " << abs_info.minimum << " to "
                << abs_info.maximum << std::endl;
    }
  }
}

// Other method implementations
