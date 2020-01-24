#include "../../src/datatypes.h"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("LoRaPayload serialization", "[single-file]")
{
  uint8_t buf[LoRaPayload::size()];

  LoRaPayload payload(123, 456, 789);
  LoRaPayload::toByteStream(&buf[0], sizeof(buf), payload);
  REQUIRE(buf[0] == 'R');
  REQUIRE(buf[1] == 'P');
  REQUIRE(buf[2] == 'O');
  REQUIRE(buf[3] == 123);
  REQUIRE(buf[4] == 0);
  REQUIRE(buf[5] == SensorData);
  REQUIRE(buf[6] == 0xc8);
  REQUIRE(buf[7] == 0x01);
  REQUIRE(buf[8] == 0x00);
  REQUIRE(buf[9] == 0x00);
  REQUIRE(buf[10] == 0x15);
  REQUIRE(buf[11] == 0x03);
  REQUIRE(buf[12] == 0x00);
  REQUIRE(buf[13] == 0x00);
  REQUIRE(buf[14] == 0x00);
  REQUIRE(buf[15] == 0x00);

  LoRaPayload restoredPayload;
  LoRaPayload::fromByteStream(&buf[0], sizeof(buf), restoredPayload);
  REQUIRE(restoredPayload.signatureOK() == true);
  REQUIRE(restoredPayload.nodeID == 123);
  REQUIRE(restoredPayload.cmd == SensorData);
  REQUIRE(restoredPayload.sensordata.value == 456);
  REQUIRE(restoredPayload.sensordata.nonce == 789);
}
