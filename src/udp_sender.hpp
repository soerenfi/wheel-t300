// udp_sender.h

#ifndef UDP_SENDER_H
#define UDP_SENDER_H

#include <netinet/in.h>

#include <cstdint>
#include <string>

class UDPSender {
 public:
  UDPSender();
  bool Initialize(const std::string& server_ip, int server_port);
  bool SendData(const void* data, size_t size);
  ~UDPSender();

 private:
  std::string server_ip_;
  int server_port_;
  int socket_fd_;
  struct sockaddr_in server_address_;
};

#endif  // UDP_SENDER_H
