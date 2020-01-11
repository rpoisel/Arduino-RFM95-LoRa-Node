#ifndef RH_RF95_H_
#define RH_RF95_H_

#include "../../src/datatypes.h"

#include <cstddef>
#include <cstdint>

class RH_RF95
{
  public:
  RH_RF95(uint8_t slaveSelectPin, uint8_t interruptPin)
      : cntRead{0}, cntWrite{0}, invokeReceiveCb(true)
  {
    LoRaPayload const sPayload = LoRaPayload(0x01 /* nodeID */, 1234 /* nonce */);
    LoRaPayload::toByteStream(&cPayload[0], sizeof(cPayload), sPayload);
  }
  bool init()
  {
    return true;
  }
  bool setFrequency(float centre)
  {
    return true;
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
  void setPayloadCRC(bool on)
  {
  }
  void setCodingRate4(int rate)
  {
  }
  bool available()
  {
    return invokeReceiveCb;
  }
  bool waitAvailableTimeout(uint16_t timeout)
  {
    return available();
  }

  bool recv(uint8_t* buf, uint8_t* len)
  {
    if (invokeReceiveCb)
    {
      *len = sizeof(cPayload);
      for (size_t cnt = 0; cnt < *len; cnt++)
      {
        cntRead++;
        buf[cnt] = cPayload[cnt];
      }
      return true;
    }
    *len = 0;
    return true;
  }
  void send(uint8_t const* buf, size_t len)
  {
    cntWrite++;
  }
  bool waitPacketSent()
  {
    return true;
  }
  void setTxPower(int8_t power, bool useRFO = false)
  {
  }
  void setModeRx()
  {
  }
  void setModeTx()
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
  uint8_t cPayload[LoRaPayload::size()];

  // testing data
  size_t cntRead;
  size_t cntWrite;
  bool invokeReceiveCb;
};

#endif /* RH_RF95_H_ */
