#ifndef UTIL_H_
#define UTIL_H_

#include <cstddef>
#include <cstdint>

#if defined(ARDUINO)
void* operator new(size_t s, void* b)
{
  return b;
}
#endif

namespace Util
{

template <typename T>
static constexpr T static_max(T a)
{
  return a;
}

template <typename T, typename... Args>
static constexpr T static_max(T a, T b)
{
  return a > b ? a : b;
}

template <typename T, typename... Args>
static constexpr T static_max(T a, T b, Args... args)
{
  return a > b ? a : (b > static_max(args...) ? b : static_max(args...));
}

template <size_t N, typename... Args>
class MultiBuffer
{
  public:
  MultiBuffer() : curBuf{0}, buf{}
  {
  }
  void* getNewBuf()
  {
    auto oldIdx = curBuf;
    curBuf = (curBuf + 1) % N;
    return &buf[oldIdx][0];
  }
  size_t getBufSize() const
  {
    return sizeof(buf);
  }

  private:
  size_t curBuf;
  uint8_t buf[N][static_max(sizeof(Args)...)];
};

} // namespace Util

#endif /* UTIL_H_ */
