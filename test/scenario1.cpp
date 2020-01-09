#include <Arduino.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Default behavior of delayer functionality", "[single-file]")
{
  setup();
  for (size_t cnt = 0; cnt < 100; cnt++)
  {
    loop();
  }
}
