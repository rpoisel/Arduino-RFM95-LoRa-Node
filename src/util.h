#ifndef UTIL_H_
#define UTIL_H_

#include <Arduino.h>

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

} // namespace Util

#if defined(ARDUINO)
void* operator new(size_t s, void* b)
{
  return b;
}
#endif

#endif /* UTIL_H_ */
