#include <Arduino.h>
#include <LoRa.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

static constexpr size_t const NUM_STATES = 3;

using FrameworkFunc = void (*)(void);

static struct
{
  FrameworkFunc func;
  FrameworkFunc pre;
  unsigned long curTime;
  size_t cntRead;
  size_t cntWrite;
} const TEST_INPUT[] = {
    {&setup, []() {}, 0, 0, 0},
    {&loop, []() {}, 1001, 0, 1},
    {&loop, []() {}, 1001, 1, 1},
    {&loop, []() {}, 1001, 1, 1},
    {&loop, []() { LoRa.setInvokeReceiveCb(false); }, 2002, 1, 2},
    {&loop, []() {}, 2002, 1, 2},
    {&loop, []() { LoRa.setInvokeReceiveCb(true); }, 7007, 2, 2},
    {&loop, []() {}, 7007, 2, 2},
    {&loop, []() {}, 8008, 2, 3},
};

TEST_CASE("Default behavior of delayer functionality", "[single-file]")
{
  for (auto const& input : TEST_INPUT)
  {
    setMillis(input.curTime);
    (*input.pre)();
    (*input.func)();
    REQUIRE(LoRa.getCntRead() == input.cntRead * LoRaPayload::size());
    REQUIRE(LoRa.getCntWrite() == input.cntWrite);
  }
}
