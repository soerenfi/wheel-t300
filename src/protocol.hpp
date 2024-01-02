#ifndef WHEEL_PROTOCOL_HPP
#define WHEEL_PROTOCOL_HPP
#include <cstdint>
#include <limits>

#pragma pack(push, 1)  // Pack the data structure tightly

struct WheelData {
  uint16_t steering_angle{std::numeric_limits<uint16_t>::max() /
                          2};    // Steering angle in degrees (0-800)
  uint8_t pedal1_percentage{0};  // Pedal 1 percentage (0-100)
  uint8_t pedal2_percentage{0};  // Pedal 2 percentage (0-100)
};

#pragma pack(pop)  // Restore default packing

#endif  // WHEEL_PROTOCOL_HPP
