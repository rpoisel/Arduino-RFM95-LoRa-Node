#include "../src/util.h"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("MultiBuffer behavior", "[single-file]")
{
  Util::MultiBuffer<1, uint8_t, uint16_t, uint32_t, uint64_t> mb1;
  REQUIRE(mb1.getBufSize() == sizeof(uint64_t));

  Util::MultiBuffer<1, uint64_t, uint32_t, uint16_t, uint8_t> mb2;
  REQUIRE(mb2.getBufSize() == sizeof(uint64_t));

  Util::MultiBuffer<1, uint64_t, uint32_t, uint16_t> mb3;
  REQUIRE(mb3.getBufSize() == sizeof(uint64_t));
}

TEST_CASE("MultiBuffer usage", "[single-file]")
{
  Util::MultiBuffer<2, uint8_t, uint16_t, uint32_t, uint64_t> mb1;
  auto retVal1 = mb1.getNewBuf();
  auto retVal2 = mb1.getNewBuf();
  auto retVal3 = mb1.getNewBuf();
  auto retVal4 = mb1.getNewBuf();

  REQUIRE(retVal1 == retVal3);
  REQUIRE(retVal2 == retVal4);
  REQUIRE(reinterpret_cast<uint8_t const*>(retVal2) ==
          reinterpret_cast<uint8_t const*>(retVal1) + sizeof(uint64_t));
}
