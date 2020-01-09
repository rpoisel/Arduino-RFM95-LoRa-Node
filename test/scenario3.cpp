#include "../src/util.h"

#include <iostream>

int main(void)
{
  Util::MultiBuffer<1, uint8_t, uint16_t, uint32_t, uint64_t> mb1;
  std::cout << mb1.getBufSize() << std::endl;

  Util::MultiBuffer<1, uint64_t, uint32_t, uint16_t, uint8_t> mb2;
  std::cout << mb2.getBufSize() << std::endl;

  Util::MultiBuffer<1, uint64_t, uint32_t, uint16_t> mb3;
  std::cout << mb3.getBufSize() << std::endl;

  return 0;
}
