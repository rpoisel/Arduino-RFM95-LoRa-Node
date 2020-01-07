#include <LoRa.h>

LoRaPayload const LoRaClass::sPayload = LoRaPayload(0x01 /* nodeID */, 1234 /* nonce */);

LoRaClass LoRa;
