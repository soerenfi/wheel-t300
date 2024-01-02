#include "wheel_driver.h"

int main() {
  WheelDriver driver("/dev/input/js0");
  driver.Run();
  return 0;
}
