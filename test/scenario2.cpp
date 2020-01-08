#include "../src/datatypes.h"

#include <iostream>

int main(void)
{
  uint8_t buf[LoRaPayload::size()];

  LoRaPayload payload(123, 456, 789);
  LoRaPayload::toByteStream(&buf[0], sizeof(buf), payload);
  std::cout << (buf[0] == 'R') << std::endl;
  std::cout << (buf[1] == 'P') << std::endl;
  std::cout << (buf[2] == 'O') << std::endl;
  std::cout << (buf[3] == 123) << std::endl;
  std::cout << (buf[4] == 0) << std::endl;
  std::cout << (buf[5] == SensorData) << std::endl;
  std::cout << (buf[6] == 0xc8) << std::endl;
  std::cout << (buf[7] == 0x01) << std::endl;
  std::cout << (buf[8] == 0x00) << std::endl;
  std::cout << (buf[9] == 0x00) << std::endl;
  std::cout << (buf[10] == 0x15) << std::endl;
  std::cout << (buf[11] == 0x03) << std::endl;
  std::cout << (buf[12] == 0x00) << std::endl;
  std::cout << (buf[13] == 0x00) << std::endl;
  std::cout << (buf[14] == 0x00) << std::endl;
  std::cout << (buf[15] == 0x00) << std::endl;

  std::cout << "==" << std::endl;

  LoRaPayload restoredPayload;
  LoRaPayload::fromByteStream(&buf[0], sizeof(buf), restoredPayload);
  std::cout << restoredPayload.signatureOK() << std::endl;
  std::cout << (restoredPayload.nodeID == 123) << std::endl;
  std::cout << (restoredPayload.cmd == SensorData) << std::endl;
  std::cout << (restoredPayload.sensordata.value == 456) << std::endl;
  std::cout << (restoredPayload.sensordata.nonce == 789) << std::endl;

  return 0;
}
