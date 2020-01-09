#ifndef LORA_H_
#define LORA_H_

#include "../../src/datatypes.h"

#include <cstddef>
#include <cstdint>

using ReceiveCb = void (*)(void* context, int len);

class LoRaClass
{
  public:
  LoRaClass() : pos{0}, cntRead{0}, cntWrite{0}, invokeReceiveCb(true)
  {
    LoRaPayload::toByteStream(&cPayload[0], sizeof(cPayload), sPayload);
  }
  bool begin(double freq)
  {
    return true;
  }
  void setPins(int ss, int reset, int dio0)
  {
  }
  void setSpreadingFactor(int factor)
  {
  }
  void setPreambleLength(int len)
  {
  }
  void setSignalBandwidth(uint32_t freq)
  {
  }
  void disableCrc()
  {
  }
  void setCodingRate4(int rate)
  {
  }
  void onReceive(ReceiveCb cb, void* context)
  {
    this->receiveCb = cb;
    this->context = context;
  }
  void receive()
  {
    if (invokeReceiveCb)
    {
      this->receiveCb(context, sizeof(cPayload));
    }
  }
  uint8_t read()
  {
    cntRead++;
    auto retVal = cPayload[pos];
    pos = (pos + 1) % sizeof(cPayload);
    return retVal;
  }
  void write(uint8_t const* buf, size_t len)
  {
    cntWrite++;
  }
  void beginPacket()
  {
  }
  void endPacket()
  {
  }

  // testing methods
  size_t getCntRead() const
  {
    return cntRead;
  }
  size_t getCntWrite() const
  {
    return cntWrite;
  }
  void setInvokeReceiveCb(bool invoke)
  {
    invokeReceiveCb = invoke;
  }

  private:
  static LoRaPayload const sPayload;

  ReceiveCb receiveCb;
  void* context;

  size_t pos;
  uint8_t cPayload[LoRaPayload::size()];

  // testing data
  size_t cntRead;
  size_t cntWrite;
  bool invokeReceiveCb;
};

extern LoRaClass LoRa;

#endif /* LORA_H_ */
