#include "joystick_event_reader.h"
#include <iostream>

class JoystickDriver {
public:
    explicit JoystickDriver(const std::string& device_path) 
        : device_path_(device_path), event_reader_(device_path), axis1_value_(0), axis2_value_(0) {}

    void Run() {
        if (!event_reader_.OpenDevice()) {
            std::cerr << "Failed to open device " << device_path_ << std::endl;
            return;
        }

        std::cout << "Reading joystick input from " << device_path_ << std::endl;

        js_event event;
        while (true) {
            if (!event_reader_.ReadEvent(event)) {
                std::cerr << "Error reading from device." << std::endl;
                break;
            }

            if (event.type == JS_EVENT_AXIS || event.type == JS_EVENT_BUTTON) {
                PrintEvent(event);
            }
        }
    }

private:
    std::string device_path_;
    JoystickEventReader event_reader_;
    int axis1_value_;
    int axis2_value_;

    void PrintEvent(const js_event& event) {
        if (event.type == JS_EVENT_AXIS) {
            if (event.number == 0) { // Steering axis
                double angle = MapSteeringAxisValueToAngle(event.value);
                std::cout << "Steering Axis (Physical Value): " << angle << " degrees" << std::endl;
            } else if (event.number == 1) { // Axis 1
                axis1_value_ = event.value;
                double percentage = MapAxisValueToPercentage(axis1_value_);
                std::cout << "Axis 1: " << percentage << "%" << std::endl;
            } else if (event.number == 2) { // Axis 2
                axis2_value_ = event.value;
                double percentage = MapAxisValueToPercentage(axis2_value_);
                std::cout << "Axis 2: " << percentage << "%" << std::endl;
            } else {
                std::cout << "Axis " << static_cast<int>(event.number) << ": " << event.value << std::endl;
            }
        } else if (event.type == JS_EVENT_BUTTON) {
            std::cout << "Button " << static_cast<int>(event.number) << ": " << (event.value ? "Pressed" : "Released") << std::endl;
        }
    }

    double MapSteeringAxisValueToAngle(int value) {
        const int InputMin = -32767;
        const int InputMax = 32767;
        const int OutputMin = -400;
        const int OutputMax = 400;

        return (static_cast<double>(value - InputMin) / (InputMax - InputMin)) * (OutputMax - OutputMin) + OutputMin;
    }

    double MapAxisValueToPercentage(int value) {
        const int InputMin = -32767;
        const int InputMax = 32767;
        const int OutputMin = 0;
        const int OutputMax = 100;

        return (static_cast<double>(value - InputMin) / (InputMax - InputMin)) * (OutputMax - OutputMin);
    }
};

int main() {
    JoystickDriver driver("/dev/input/js0");
    driver.Run();
    return 0;
}
