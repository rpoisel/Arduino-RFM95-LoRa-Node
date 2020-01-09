#include <Arduino.h>
#include <LoRa.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

static constexpr size_t const NUM_STATES = 3;

TEST_CASE("Default behavior of delayer functionality", "[single-file]")
{
  size_t cntRead = 0;
  size_t cntWrite = 0;

  setMillis(0);

  setup();
  REQUIRE(LoRa.getCntRead() == 0);
  REQUIRE(LoRa.getCntWrite() == 0);

  setMillis(1001);

  loop();
  cntWrite++;
  REQUIRE(LoRa.getCntRead() == cntRead * LoRaPayload::size());
  REQUIRE(LoRa.getCntWrite() == cntWrite);

  loop();
  cntRead++;
  REQUIRE(LoRa.getCntRead() == cntRead * LoRaPayload::size());
  REQUIRE(LoRa.getCntWrite() == cntWrite);

  loop();
  REQUIRE(LoRa.getCntRead() == cntRead * LoRaPayload::size());
  REQUIRE(LoRa.getCntWrite() == cntWrite);

  setMillis(2002);
  LoRa.setInvokeReceiveCb(false);

  loop();
  cntWrite++;
  REQUIRE(LoRa.getCntRead() == cntRead * LoRaPayload::size());
  REQUIRE(LoRa.getCntWrite() == cntWrite);

  loop();
  REQUIRE(LoRa.getCntRead() == cntRead * LoRaPayload::size());
  REQUIRE(LoRa.getCntWrite() == cntWrite);

  setMillis(7007);
  LoRa.setInvokeReceiveCb(true);

  loop();
  cntRead++;
  REQUIRE(LoRa.getCntRead() == cntRead * LoRaPayload::size());
  REQUIRE(LoRa.getCntWrite() == cntWrite);

  loop();
  REQUIRE(LoRa.getCntRead() == cntRead * LoRaPayload::size());
  REQUIRE(LoRa.getCntWrite() == cntWrite);

  setMillis(8008);

  loop();
  cntWrite++;
  REQUIRE(LoRa.getCntRead() == cntRead * LoRaPayload::size());
  REQUIRE(LoRa.getCntWrite() == cntWrite);
}
