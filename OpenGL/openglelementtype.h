#ifndef OPENGLELEMENTTYPE_H
#define OPENGLELEMENTTYPE_H OpenGLElementType

#include <cstdint>
#include <cstdlib>

enum class OpenGLElementType
{
  Byte                            = 0x1400,
  UnsignedByte                    = 0x1401,
  Short                           = 0x1402,
  UnsignedShort                   = 0x1403,
  Integer                         = 0x1404,
  UnsignedInteger                 = 0x1405,
  Float                           = 0x1406,
  TwoBytes                        = 0x1407,
  ThreeBytes                      = 0x1408,
  FourBytes                       = 0x1409,
  Double                          = 0x140A
};

size_t OpenGLElementSize(OpenGLElementType type);

#endif // OPENGLELEMENTTYPE_H
