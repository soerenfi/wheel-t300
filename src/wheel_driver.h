#ifndef WHEEL_DRIVER_H
#define WHEEL_DRIVER_H

#include "wheel_event_reader.h"
#include <linux/joystick.h>
#include <string>
#include <iostream>

class WheelDriver {
public:
    explicit WheelDriver(const std::string& device_path);
    void Run();

private:
    std::string device_path_;
    WheelEventReader event_reader_;
    int axis1_value_;
    int axis2_value_;

    void ProcessEvent(const js_event& event);
    void HandleAxisEvent(const js_event& event);
    void HandleButtonEvent(const js_event& event);
    double MapAxisValueToPercentage(int value);
    double MapSteeringAxisValueToAngle(int value);
};

#endif // WHEEL_DRIVER_H
