#include "wheel_driver.h"

#include <unistd.h>

#include <chrono>

#include "protocol.hpp"

WheelDriver::WheelDriver(const std::string& device_path)
    : device_path_(device_path),
      event_reader_(device_path),
      axis1_value_(0),
      axis2_value_(0) {}

void WheelDriver::Run() {
  if (!event_reader_.OpenDevice()) {
    std::cerr << "Failed to open device " << device_path_ << std::endl;
    return;
  }

  event_reader_.QueryDeviceCapabilities();

  if (event_reader_.HasForceFeedbackSupport()) {
    std::cout << "Force feedback (rumble) supported." << std::endl;
  } else {
    std::cout << "Force feedback (rumble) not supported." << std::endl;
  }
  auto currentTime = std::chrono::steady_clock::now();

  js_event event;
  auto startTime = std::chrono::steady_clock::now();

  while (true) {
    if (event_reader_.ReadEvent(event)) {
      ProcessEvent(event);
    }

    static uint64_t loop_count{0};
    loop_count++;  // Increment the loop_count_ with each iteration

    // Print execution frequency every N iterations
    if (loop_count % 1000 ==
        0) {  // Adjust N as needed for your desired frequency
      auto currentTime = std::chrono::steady_clock::now();
      auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(
          currentTime - startTime);
      double frequency = static_cast<double>(loop_count) / elapsedTime.count();
      std::cout << "Execution Frequency: " << frequency << " Hz" << std::endl;
    }

    SendDataOverUDP();

    // Optionally, introduce a delay to control the sending rate
    usleep(10000);  // 10 milliseconds delay (adjust as needed)
  }
}

bool WheelDriver::InitializeUDPSender(const std::string& server_ip,
                                      int server_port) {
  return udp_sender_.Initialize(server_ip, server_port);
}

void WheelDriver::SendDataOverUDP() {
  // Send the data over UDP
  udp_sender_.SendData(&wheel_data_, sizeof(wheel_data_));
}

void WheelDriver::ProcessEvent(const js_event& event) {
  switch (event.type) {
    case JS_EVENT_AXIS: {
      HandleAxisEvent(event);
      break;
    };
    case JS_EVENT_BUTTON: {
      HandleButtonEvent(event);
      break;
    }
    default:
      break;
  };
}

void WheelDriver::HandleAxisEvent(const js_event& event) {
  // Axis event handling logic
  // Map axis values to meaningful representations
  if (event.number == 0) {  // Steering axis
    double angle = MapSteeringAxisValueToAngle(event.value);
    // std::cout << "Steering Axis (Physical Value): " << angle << " degrees"
    //           << std::endl;
    wheel_data_.steering_angle = MapAngleToUint(angle);
  } else if (event.number == 1) {  // Pedals
    double percentage = MapAxisValueToPercentage(event.value);
    // std::cout << "Pedal " << static_cast<int>(event.number) << ": "
    //           << percentage << "%" << std::endl;
    wheel_data_.pedal1_percentage = MapPercentageToUint(percentage);
  } else if (event.number == 2) {  // Pedals
    double percentage = MapAxisValueToPercentage(event.value);
    // std::cout << "Pedal " << static_cast<int>(event.number) << ": "
    //           << percentage << "%" << std::endl;
    wheel_data_.pedal2_percentage = MapPercentageToUint(percentage);
  }
}

void WheelDriver::HandleButtonEvent(const js_event& event) {
  // Button event handling logic
  std::cout << "Button " << static_cast<int>(event.number) << ": "
            << (event.value ? "Pressed" : "Released") << std::endl;
}

double WheelDriver::MapAxisValueToPercentage(int value) {
  const int InputMin = -32767;
  const int InputMax = 32767;
  const int OutputMin = 100;  // 100% when fully pressed
  const int OutputMax = 0;    // 0% when unpressed

  return (static_cast<double>(value - InputMin) / (InputMax - InputMin)) *
             (OutputMax - OutputMin) +
         OutputMin;
}

double WheelDriver::MapSteeringAxisValueToAngle(int value) {
  const int InputMin = -32767;
  const int InputMax = 32767;
  const int OutputMin = -400;  // -400 degrees
  const int OutputMax = 400;   // 400 degrees

  return (static_cast<double>(value - InputMin) / (InputMax - InputMin)) *
             (OutputMax - OutputMin) +
         OutputMin;
}

uint16_t WheelDriver::MapAngleToUint(double angle) {
  // Map the angle (e.g., -400 to 400) to a uint value (0-65535)
  return static_cast<uint16_t>((angle + 400.0) * (65535.0 / 800.0));
}

uint8_t WheelDriver::MapPercentageToUint(double percentage) {
  // Map the percentage (e.g., 0-100) to a uint value (0-255)
  return static_cast<uint8_t>(percentage * 2.55);
}
