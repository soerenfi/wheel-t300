#ifndef JOYSTICK_EVENT_READER_H
#define JOYSTICK_EVENT_READER_H

#include <linux/joystick.h>
#include <string>
#include <fstream>

class JoystickEventReader {
public:
    explicit JoystickEventReader(const std::string& device_path) : device_path_(device_path) {}

    bool OpenDevice() {
        device_stream_.open(device_path_, std::ios::in | std::ios::binary);
        return device_stream_.is_open();
    }

    bool ReadEvent(js_event& event) {
        if (!device_stream_.read(reinterpret_cast<char*>(&event), sizeof(js_event))) {
            return false;
        }
        return true;
    }

private:
    std::string device_path_;
    std::ifstream device_stream_;
};

#endif // JOYSTICK_EVENT_READER_H
