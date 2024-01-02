#ifndef WHEEL_DRIVER_H
#define WHEEL_DRIVER_H

#include <linux/joystick.h>

#include <iostream>
#include <string>

#include "protocol.hpp"
#include "udp_sender.hpp"
#include "wheel_event_reader.h"

class WheelDriver {
 public:
  explicit WheelDriver(const std::string& device_path);
  void Run();
  bool InitializeUDPSender(const std::string& server_ip, int server_port);
  void SendDataOverUDP();
  uint16_t MapAngleToUint(double angle);
  uint8_t MapPercentageToUint(double percentage);

 private:
  std::string device_path_;
  WheelEventReader event_reader_;
  int axis1_value_;
  int axis2_value_;

  UDPSender udp_sender_;
  WheelData wheel_data_;  // Store the data to send

  void ProcessEvent(const js_event& event);
  void HandleAxisEvent(const js_event& event);
  void HandleButtonEvent(const js_event& event);
  double MapAxisValueToPercentage(int value);
  double MapSteeringAxisValueToAngle(int value);
};

#endif  // WHEEL_DRIVER_H
