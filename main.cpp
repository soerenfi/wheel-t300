#include "joystick_event_reader.h"
#include <iostream>

class JoystickDriver {
public:
    explicit JoystickDriver(const std::string& device_path) 
        : device_path_(device_path), event_reader_(device_path) {}

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

    void PrintEvent(const js_event& event) {
        if (event.type == JS_EVENT_AXIS) {
            if (event.number == 0) { // Assuming axis 0 is the steering axis
                double angle = MapAxisValueToAngle(event.value);
                std::cout << "Steering Axis: " << angle << " degrees" << std::endl;
            } else {
                std::cout << "Axis " << static_cast<int>(event.number) << ": " << event.value << std::endl;
            }
        } else if (event.type == JS_EVENT_BUTTON) {
            std::cout << "Button " << static_cast<int>(event.number) << ": " << (event.value ? "Pressed" : "Released") << std::endl;
        }
    }

    double MapAxisValueToAngle(int value) {
        const int InputMin = -32767;
        const int InputMax = 32767;
        const int OutputMin = -360;
        const int OutputMax = 360;

        return (static_cast<double>(value - InputMin) / (InputMax - InputMin)) * (OutputMax - OutputMin) + OutputMin;
    }
};

int main() {
    JoystickDriver driver("/dev/input/js0");
    driver.Run();
    return 0;
}
