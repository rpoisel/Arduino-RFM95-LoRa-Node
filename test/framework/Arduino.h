#ifndef ARDUINO_H_
#define ARDUINO_H_

#include <cstddef>
#include <cstdint>

#include <iostream>

#ifdef __cplusplus
extern "C" {
void setup();
void loop();
}
#endif

using byte = uint8_t;

constexpr uint8_t const LOW = 0x00;
constexpr uint8_t const HIGH = 0x01;
constexpr uint8_t const INPUT = 0x00;
constexpr uint8_t const OUTPUT = 0x01;

void setMillis(unsigned long val);
unsigned long millis();
void delay(unsigned long);
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);

enum NumberFormat : uint8_t
{
  DEC
};

class SerialClass
{
  public:
  void begin(uint32_t baud)
  {
  }
  bool operator!() const
  {
    return false;
  }
  void println(char const* msg)
  {
    std::cout << msg << std::endl;
  }
  void print(char const* msg)
  {
    std::cout << msg << std::flush;
  }
  void println(size_t num, NumberFormat fmt)
  {
    std::cout << num << std::flush;
  }
  void println(uint32_t num)
  {
    std::cout << num << std::endl;
  }
};

extern SerialClass Serial;

#endif /* ARDUINO_H_ */
