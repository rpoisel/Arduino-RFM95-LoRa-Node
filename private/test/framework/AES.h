#ifndef AES_H_
#define AES_H_

#include <cstddef>
#include <cstdint>

class AES256
{
  public:
  static constexpr size_t const BLOCK_SIZE = 16;
  size_t keySize() const
  {
    return 32;
  }
  void setKey(uint8_t const* key, size_t keySize)
  {
  }
  void encryptBlock(uint8_t* dst, uint8_t const* src)
  {
    for (size_t cnt = 0; cnt < BLOCK_SIZE; cnt++)
    {
      dst[cnt] = src[cnt];
    }
  }
  void decryptBlock(uint8_t* dst, uint8_t const* src)
  {
    for (size_t cnt = 0; cnt < BLOCK_SIZE; cnt++)
    {
      dst[cnt] = src[cnt];
    }
  }
};

#endif /* AES_H_ */
