#include <Arduino.h>

int main(void)
{
  setup();
  for (size_t cnt = 0; cnt < 100; cnt++)
  {
    loop();
  }
}
