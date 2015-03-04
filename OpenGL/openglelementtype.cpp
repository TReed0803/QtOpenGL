#include "openglelementtype.h"

size_t OpenGLElementSize(OpenGLElementType type)
{
  switch(type)
  {
  case OpenGLElementType::Byte:
    return sizeof(int8_t);
  case OpenGLElementType::UnsignedByte:
    return sizeof(uint8_t);
  case OpenGLElementType::Short:
    return sizeof(int16_t);
  case OpenGLElementType::UnsignedShort:
    return sizeof(uint16_t);
  case OpenGLElementType::Integer:
    return sizeof(int32_t);
  case OpenGLElementType::UnsignedInteger:
    return sizeof(uint32_t);
  case OpenGLElementType::Float:
    return sizeof(float);
  case OpenGLElementType::TwoBytes:
    return sizeof(int8_t) * 2;
  case OpenGLElementType::ThreeBytes:
    return sizeof(int8_t) * 3;
  case OpenGLElementType::FourBytes:
    return sizeof(int8_t) * 4;
  case OpenGLElementType::Double:
    return sizeof(double);
  }
  return 0;
}
