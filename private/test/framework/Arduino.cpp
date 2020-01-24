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

void delay(unsigned long)
{
}

void pinMode(uint8_t pin, uint8_t mode)
{
}

void digitalWrite(uint8_t pin, uint8_t value)
{
}

SerialClass Serial;
