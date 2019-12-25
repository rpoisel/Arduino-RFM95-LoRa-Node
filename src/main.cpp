#include <LoRa.h>

constexpr int const LORA_SS = 10;
constexpr int const LORA_RESET = 9;
constexpr int const LORA_DIO0 = 7;

static size_t counter = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  Serial.println("LoRa Sender");

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
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(1000);
}
