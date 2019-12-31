#include <AES.h>
#include <LoRa.h>

constexpr int const LORA_SS = 10;
constexpr int const LORA_RESET = 9;
constexpr int const LORA_DIO0 = 7;
constexpr uint8_t const AES_KEY[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                     0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

static size_t counter = 0;
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
  uint8_t buffer[16]{'\0'};

  String msg("Hello ");
  msg += counter;

  if (msg.length() <= sizeof(buffer) - 1)
  {
    aes.encryptBlock(buffer, reinterpret_cast<uint8_t const*>(msg.c_str()));

    Serial.print("Sending packet: ");
    Serial.println(counter);

    LoRa.beginPacket();
    LoRa.write(&buffer[0], sizeof(buffer));
    LoRa.endPacket();
  }

  counter++;

  delay(1000);
}
