#include "wheel_driver.h"

WheelDriver::WheelDriver(const std::string& device_path) 
    : device_path_(device_path), event_reader_(device_path), axis1_value_(0), axis2_value_(0) {}

void WheelDriver::Run() {
    if (!event_reader_.OpenDevice()) {
        std::cerr << "Failed to open device " << device_path_ << std::endl;
        return;
    }

    event_reader_.QueryDeviceCapabilities();

    js_event event;
    while (true) {
        if (event_reader_.ReadEvent(event)) {
            ProcessEvent(event);
        }
    }
}

void WheelDriver::ProcessEvent(const js_event& event) {
    if (event.type == JS_EVENT_AXIS) {
        HandleAxisEvent(event);
    } else if (event.type == JS_EVENT_BUTTON) {
        HandleButtonEvent(event);
    }
}

void WheelDriver::HandleAxisEvent(const js_event& event) {
    // Axis event handling logic
    // Map axis values to meaningful representations
    if (event.number == 0) { // Steering axis
        double angle = MapSteeringAxisValueToAngle(event.value);
        std::cout << "Steering Axis (Physical Value): " << angle << " degrees" << std::endl;
    } else if (event.number == 1 || event.number == 2) { // Pedals
        double percentage = MapAxisValueToPercentage(event.value);
        std::cout << "Pedal " << static_cast<int>(event.number) << ": " << percentage << "%" << std::endl;
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

    return (static_cast<double>(value - InputMin) / (InputMax - InputMin)) * (OutputMax - OutputMin) + OutputMin;
}

double WheelDriver::MapSteeringAxisValueToAngle(int value) {
    const int InputMin = -32767;
    const int InputMax = 32767;
    const int OutputMin = -400; // -400 degrees
    const int OutputMax = 400;  // 400 degrees

    return (static_cast<double>(value - InputMin) / (InputMax - InputMin)) * (OutputMax - OutputMin) + OutputMin;
}
