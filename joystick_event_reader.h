#ifndef JOYSTICK_EVENT_READER_H
#define JOYSTICK_EVENT_READER_H

#include <linux/joystick.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>

class JoystickEventReader {
public:
    explicit JoystickEventReader(const std::string& device_path) : device_path_(device_path), fd_(-1) {}

    bool OpenDevice() {
        fd_ = open(device_path_.c_str(), O_RDWR);
        if (fd_ < 0) {
            return false;
        }
        device_stream_.open(device_path_, std::ios::in | std::ios::binary);
        return device_stream_.is_open();
    }

    void QueryDeviceCapabilities() {
        // Corrected initialization
        unsigned long evbit[(EV_MAX + 7) / 8] = {0};

        if (ioctl(fd_, EVIOCGBIT(0, EV_MAX), evbit) < 0) {
            std::cerr << "Error querying device capabilities" << std::endl;
            return;
        }

        if (evbit[EV_FF / 8] & (1 << (EV_FF % 8))) {
            std::cout << "Device supports force feedback" << std::endl;
            // Further queries for specific types of force feedback can be done here
        } else {
            std::cout << "Device does not support force feedback" << std::endl;
        }
    }


    bool ReadEvent(js_event& event) {
        if (!device_stream_.read(reinterpret_cast<char*>(&event), sizeof(js_event))) {
            return false;
        }
        return true;
    }

    ~JoystickEventReader() {
        if (fd_ >= 0) {
            close(fd_);
        }
    }

private:
    std::string device_path_;
    std::ifstream device_stream_;
    int fd_; // File descriptor for ioctl calls
};

#endif // JOYSTICK_EVENT_READER_H
