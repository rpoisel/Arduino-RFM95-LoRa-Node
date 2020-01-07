#include <Arduino.h>

unsigned long millis()
{
  static unsigned long curTime = 0;
  auto retVal = curTime;
  curTime += 1000;
  return retVal;
}

SerialClass Serial;
