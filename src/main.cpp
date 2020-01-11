#include "data.h"
#include "datatypes.h"
#include "fsm.h"
#include "util.h"

#include <AES.h>

using namespace LoRaNode;
using Util::MultiBuffer;
using Util::static_max;

class RequestNonce : public State
{
  public:
  RequestNonce() : lastSend{millis()}
  {
  }
  void execute(FSM* fsm);
  void enter();

  private:
  unsigned long lastSend;
};

class AwaitNonce : public State
{
  public:
  AwaitNonce() : enteredState{millis()}
  {
  }
  void execute(FSM* fsm);
  void enter();

  private:
  unsigned long enteredState;
};

class SendSensorData : public State
{
  public:
  SendSensorData(LoRaNonce const& nonce) : nonce(nonce)
  {
  }
  void execute(FSM* fsm);
  void enter();

  LoRaNonce nonce;
};

constexpr uint8_t const RFM95_RST = PIN_RFM95_RST;
constexpr uint8_t const AES_KEY[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
constexpr LoRaNodeID const NODE_ID = 0x0001;

static AES256 aes;
static FSM fsm;
static MultiBuffer<2, RequestNonce, AwaitNonce, SendSensorData> multiBuf;

void setup()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  while (!Serial)
    ;

  delay(100);
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  aes.setKey(&AES_KEY[0], aes.keySize());

  while (!rf95.init())
  {
    Serial.println("Starting LoRa failed!");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1)
      ;
  }

  if (!rf95.setFrequency(868.0))
  {
    Serial.println("Could not set frequency");
    while (1)
      ;
  }
  rf95.setTxPower(23, false);
  rf95.setSpreadingFactor(7);
  rf95.setPreambleLength(8);
  rf95.setSignalBandwidth(125000);
  rf95.setPayloadCRC(false);
  rf95.setCodingRate4(5);

  fsm.nextState(new (multiBuf.getNewBuf()) RequestNonce());
}

void loop()
{
  fsm.loop();
}

void RequestNonce::enter()
{
  Serial.println("Entering RequestNonce.");
  lastSend = millis();
  rf95.setModeTx();
}

void RequestNonce::execute(FSM* fsm)
{
  // Serial.println("Executing RequestNonce.");
  if (millis() - lastSend > 1000)
  {
    LoRaPayload payload(NODE_ID);
    payload.cmd = GetNonce;

    uint8_t encrypted[sizeof(payload)];
    uint8_t cleartext[LoRaPayload::size()];
    LoRaPayload::toByteStream(cleartext, sizeof(cleartext), payload);
    aes.encryptBlock(encrypted, cleartext);

    Serial.println("Sending GET_NONCE.");

    rf95.send(&encrypted[0], LoRaPayload::size());
    delay(10);
    rf95.waitPacketSent();

    fsm->nextState(new (multiBuf.getNewBuf()) AwaitNonce());
  }
}

void AwaitNonce::enter()
{
  Serial.println("Entering AwaitNonce.");
  rf95.setModeRx();
  enteredState = millis();
}

void AwaitNonce::execute(FSM* fsm)
{
  LoRaPayload payload;
  uint8_t cleartext[LoRaPayload::size()];
  uint8_t encrypted[UINT8_MAX];
  uint8_t lenReceived = sizeof(encrypted);

  // Serial.println("Executing AwaitNonce.");
  if (rf95.waitAvailableTimeout(1000))
  {
    if (!rf95.recv(&encrypted[0], &lenReceived))
    {
      Serial.println("Recv failed ...");
    }
  }
  else
  {
    if (millis() - enteredState > 5000)
    {
      fsm->nextState(new (multiBuf.getNewBuf()) RequestNonce());
    }
    return;
  }
  if (lenReceived != LoRaPayload::size())
  {
    Serial.print("Wrong number of bytes received: ");
    Serial.println(lenReceived, DEC);
    return;
  }
  aes.decryptBlock(&cleartext[0], &encrypted[0]);
  LoRaPayload::fromByteStream(cleartext, sizeof(cleartext), payload);
  Serial.println("Message received.");
  if (!payload.signatureOK())
  {
    Serial.println("Signature check failed.");
    return;
  }
  if (payload.cmd != PutNonce)
  {
    Serial.print("Invalid command. Got: ");
    Serial.println(payload.cmd);
    return;
  }
  Serial.print("Received nonce: ");
  Serial.println(payload.nonce);
  delay(100);
  fsm->nextState(new (multiBuf.getNewBuf()) SendSensorData(payload.nonce));
}

void SendSensorData::enter()
{
  Serial.println("Entering SendSensorData.");
  rf95.setModeTx();
}

void SendSensorData::execute(FSM* fsm)
{
  // Serial.println("Executing SendSensorData.");
  fsm->nextState(new (multiBuf.getNewBuf()) RequestNonce());
}
