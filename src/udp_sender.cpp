
#include "udp_sender.hpp"

#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>  // for strerror
#include <iostream>

UDPSender::UDPSender() {}

bool UDPSender::Initialize(const std::string& server_ip, int server_port) {
  server_ip_ = server_ip;
  server_port_ = server_port;
  socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd_ == -1) {
    std::cerr << "Failed to create UDP socket: " << strerror(errno)
              << std::endl;
    return false;
  }

  // Set up server address
  memset(&server_address_, 0, sizeof(server_address_));
  server_address_.sin_family = AF_INET;
  server_address_.sin_port = htons(server_port_);
  if (inet_pton(AF_INET, server_ip_.c_str(), &server_address_.sin_addr) <= 0) {
    std::cerr << "Invalid server IP address: " << server_ip_ << std::endl;
    close(socket_fd_);
    socket_fd_ = -1;
    return false;
  }

  return true;
}

// Rest of the code remains the same

bool UDPSender::SendData(const void* data, size_t size) {
  if (socket_fd_ == -1) {
    std::cerr << "UDP sender not initialized." << std::endl;
    return false;
  }

  ssize_t bytesSent =
      sendto(socket_fd_, data, size, 0,
             reinterpret_cast<const struct sockaddr*>(&server_address_),
             sizeof(server_address_));

  if (bytesSent == -1) {
    std::cerr << "Failed to send UDP packet: " << strerror(errno) << std::endl;
    return false;
  }

  return true;
}

UDPSender::~UDPSender() {
  if (socket_fd_ != -1) {
    close(socket_fd_);
  }
}
