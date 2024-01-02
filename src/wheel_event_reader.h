#ifndef WHEEL_EVENT_READER_H
#define WHEEL_EVENT_READER_H

#include <linux/joystick.h>

#include <fstream>
#include <string>

class WheelEventReader {
 public:
  explicit WheelEventReader(const std::string& device_path);
  bool OpenDevice();
  void QueryDeviceCapabilities();
  bool ReadEvent(js_event& event);
  // Other public methods

 private:
  int fd_;
  std::string device_path_;
  std::ifstream device_stream_;
  // Other private members
};

#endif  // WHEEL_EVENT_READER_H
