#ifndef KCOMMON_H
#define KCOMMON_H

#include <cmath>

namespace Karma
{

inline static bool isAlpha(int c)
{
  return (c >= 'A' && c <= 'z');
}

inline static bool isNumeric(int c)
{
  return (c >= '0' && c <= '9') || c == '-';
}

inline static int ctoi(int c)
{
  return c - '0';
}

inline static float ctof(int c)
{
  return static_cast<float>(ctoi(c));
}

inline static int round(float f)
{
  return std::floor(f + 0.5f);
}

inline static int round(double d)
{
  return std::floor(d + 0.5);
}

}

#endif // KCOMMON_H

