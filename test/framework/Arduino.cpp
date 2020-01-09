#include <Arduino.h>

static unsigned long millisValue;

void setMillis(unsigned long val)
{
  millisValue = val;
}

unsigned long millis()
{
  return millisValue;
}

SerialClass Serial;
