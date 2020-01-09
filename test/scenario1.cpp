#include <Arduino.h>
#include <LoRa.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

static constexpr size_t const NUM_STATES = 3;

TEST_CASE("Default behavior of delayer functionality", "[single-file]")
{
  unsigned long curTime = 0;
  size_t cntRead = 0;
  size_t cntWrite = 0;

  setMillis(0);
  setup();

  REQUIRE(LoRa.getCntRead() == 0);

  for (size_t cnt = 0; cnt < 100; cnt++)
  {
    curTime += 1001;
    setMillis(curTime);
    loop();
    switch (cnt % NUM_STATES)
    {
    case 0:
      cntWrite++;
      REQUIRE(LoRa.getCntRead() == cntRead * LoRaPayload::size());
      REQUIRE(LoRa.getCntWrite() == cntWrite);
      break;
    case 1:
      cntRead++;
      REQUIRE(LoRa.getCntRead() == cntRead * LoRaPayload::size());
      REQUIRE(LoRa.getCntWrite() == cntWrite);
      break;
    case 2:
      REQUIRE(LoRa.getCntRead() == cntRead * LoRaPayload::size());
      REQUIRE(LoRa.getCntWrite() == cntWrite);
      break;
    default:
      break;
    }
  }
}
