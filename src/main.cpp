#include "datatypes.h"
#include "fsm.h"

#include <AES.h>
#include <LoRa.h>

using namespace LoRaNode;

static void onReceiveCb(void* context, int size);

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
  AwaitNonce() : encrypted{0}, lenReceived{0}, hasReceived{false}, enteredState{millis()}
  {
  }
  void execute(FSM* fsm);
  void enter();

  private:
  uint8_t encrypted[LoRaPayload::size()];
  size_t lenReceived;
  volatile bool hasReceived;
  unsigned long enteredState;

  friend void onReceiveCb(void* context, int size);
};

class SendSensorData : public State
{
  public:
  void execute(FSM* fsm);
  void enter();

  LoRaNonce nonce;
};

constexpr int const LORA_SS = PIN_LORA_SS;
constexpr int const LORA_RESET = PIN_LORA_RESET;
constexpr int const LORA_DIO0 = PIN_LORA_DIO0;
constexpr uint8_t const AES_KEY[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
constexpr LoRaNodeID const NODE_ID = 0x0001;

static AES256 aes;
static RequestNonce requestNonceState;
static AwaitNonce awaitNonceState;
static SendSensorData sendSensorDataState;
static FSM fsm;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

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
  LoRa.onReceive(onReceiveCb, &awaitNonceState);
  fsm.nextState(&requestNonceState);
}

void loop()
{
  fsm.loop();
}

void onReceiveCb(void* context, int size)
{
  if (size == 0)
  {
    return;
  }
  AwaitNonce* state = static_cast<AwaitNonce*>(context);
  for (state->lenReceived = 0; state->lenReceived < static_cast<size_t>(size) &&
                               state->lenReceived < sizeof(state->encrypted);
       state->lenReceived++)
  {
    state->encrypted[state->lenReceived] = static_cast<byte>(LoRa.read());
  }
  for (int cnt = state->lenReceived; cnt < size; cnt++)
  {
    LoRa.read();
  }
  state->hasReceived = true;
}

void RequestNonce::enter()
{
  Serial.println("Entering RequestNonce.");
  lastSend = millis();
}

void RequestNonce::execute(FSM* fsm)
{
  if (millis() - lastSend > 1000)
  {
    LoRaPayload payload(NODE_ID);
    payload.cmd = GetNonce;

    uint8_t encrypted[sizeof(payload)];
    uint8_t cleartext[LoRaPayload::size()];
    LoRaPayload::toByteStream(cleartext, sizeof(cleartext), payload);
    aes.encryptBlock(encrypted, cleartext);

    Serial.println("Sending GET_NONCE.");

    LoRa.beginPacket();
    LoRa.write(&encrypted[0], sizeof(encrypted));
    LoRa.endPacket();

    fsm->nextState(&awaitNonceState);
  }
}

void AwaitNonce::enter()
{
  Serial.println("Entering AwaitNonce.");
  enteredState = millis();
}

void AwaitNonce::execute(FSM* fsm)
{
  LoRaPayload payload;
  uint8_t cleartext[LoRaPayload::size()];

  hasReceived = false;
  LoRa.receive();
  if (!hasReceived)
  {
    if (millis() - enteredState > 5000)
    {
      fsm->nextState(&requestNonceState);
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
  sendSensorDataState.nonce = payload.nonce;
  fsm->nextState(&sendSensorDataState);
}

void SendSensorData::enter()
{
  Serial.println("Entering SendSensorData.");
}

void SendSensorData::execute(FSM* fsm)
{
  fsm->nextState(&requestNonceState);
}
