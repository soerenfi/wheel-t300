cmake_minimum_required(VERSION 3.0)
project(SteeringWheelReader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(SteeringWheelReader 
src/main.cpp
src/wheel_driver.cpp
src/wheel_event_reader.cpp
src/udp_sender.cpp
)
