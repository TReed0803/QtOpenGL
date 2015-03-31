#ifndef OPENGLTONEMAPPINGFUNCTION_H
#define OPENGLTONEMAPPINGFUNCTION_H OpenGLToneMappingFunction

#include <algorithm>

struct RgbF
{
  RgbF(float red, float green, float blue) :
    r(red), g(green), b(blue)
  {
    // Intentionally Empty
  }

  // Data
  float r;
  float g;
  float b;
};

inline RgbF operator*(const RgbF &lhs, const RgbF &rhs)
{
  return RgbF(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b);
}

inline RgbF operator*(const RgbF &lhs, float rhs)
{
  return RgbF(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs);
}

inline RgbF operator*(float lhs, const RgbF &rhs)
{
  return RgbF(rhs.r * lhs, rhs.g * lhs, rhs.b * lhs);
}

inline RgbF operator+(const RgbF &lhs, float rhs)
{
  return RgbF(lhs.r + rhs, lhs.g + rhs, lhs.b + rhs);
}

inline RgbF operator+(float lhs, const RgbF &rhs)
{
  return RgbF(rhs.r + lhs, rhs.g + lhs, rhs.b + lhs);
}

inline RgbF operator/(const RgbF &lhs, const RgbF &rhs)
{
  return RgbF(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b);
}

namespace std
{
  inline RgbF pow(const RgbF &base, float exp)
  {
    return RgbF(pow(base.r, exp), pow(base.g, exp), pow(base.b, exp));
  }
}

class OpenGLToneMappingFunction : public std::unary_function<RgbF, RgbF>
{
public:
  virtual RgbF operator()(RgbF input) const = 0;
};

class OpenGLStandardToneMapping : public OpenGLToneMappingFunction
{
public:
  OpenGLStandardToneMapping(float exposure, float contrast);
  virtual RgbF operator()(RgbF input) const;
private:
  float m_exposure, m_contrast;
};

#endif // OPENGLTONEMAPPINGFUNCTION_H
