#include "datatypes.h"

#include <AES.h>
#include <LoRa.h>

constexpr int const LORA_SS = 10;
constexpr int const LORA_RESET = 9;
constexpr int const LORA_DIO0 = 7;
constexpr uint8_t const AES_KEY[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
constexpr LoRaNodeID const NODE_ID = 0x0001;

static uint32_t counter = 0;
static AESTiny256 aes;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  Serial.println("LoRa Sender");
  aes.setKey(&AES_KEY[0], aes.keySize());

  if (!LoRa.begin(868E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  LoRa.setPins(LORA_SS, LORA_RESET, LORA_DIO0);
  LoRa.setSpreadingFactor(7);
  LoRa.setPreambleLength(8);
  LoRa.setSignalBandwidth(125000);
  LoRa.disableCrc();
  LoRa.setCodingRate4(5);
}

void loop()
{
  LoRaPayload payload(NODE_ID);
  payload.cmd = SensorData;
  payload.sensordata.value = counter;
  uint8_t encrypted[sizeof(payload)];
  aes.encryptBlock(encrypted, reinterpret_cast<uint8_t const*>(&payload));

  Serial.print("Sending packet: ");
  Serial.println(counter);

  LoRa.beginPacket();
  LoRa.write(&encrypted[0], sizeof(encrypted));
  LoRa.endPacket();

  counter++;

  delay(1000);
}
