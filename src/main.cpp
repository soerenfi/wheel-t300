#include "wheel_driver.h"

int main() {
  WheelDriver driver("/dev/input/js0");
  driver.InitializeUDPSender("127.0.0.1", 12345);
  driver.Run();
  return 0;
}
