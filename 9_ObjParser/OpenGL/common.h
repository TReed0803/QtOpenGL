#ifndef COMMON_H
#define COMMON_H

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

#endif // COMMON_H

