#ifndef KCOMMON_H
#define KCOMMON_H KCommon

#include <cmath>

namespace Karma
{

inline static bool isUpper(int c)
{
  return (c >= 'A' && c <= 'Z');
}

inline static bool isLower(int c)
{
  return (c >= 'a' && c <= 'z');
}

inline static int toLower(int c)
{
  if (isUpper(c))
  {
    return c + ('a' - 'A');
  }
  return c;
}

inline static int toUpper(int c)
{
  if (isLower(c))
  {
    return c - ('a' - 'A');
  }
  return c;
}

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
  return static_cast<int>(std::floor(f + 0.5f));
}

inline static int round(double d)
{
  return static_cast<int>(std::floor(d + 0.5));
}

}

#endif // KCOMMON_H

