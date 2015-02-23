#ifndef OPENGLSTORAGE_H
#define OPENGLSTORAGE_H OpenGLStorage
#include <QtOpenGL/QGL>

enum class OpenGLInternalFormat
{
  R8                    = 0x8229,
  R8SNorm               = 0x8F94,
  R16                   = 0x822A,
  R16SNorm              = 0x8F98,
  Rg8                   = 0x822B,
  Rg8SNorm              = 0x8F95,
  Rg16                  = 0x822C,
  Rg16SNorm             = 0x8F99,
  Rg3B2                 = 0x2A10,
  Rgb4                  = 0x804F,
  Rgb5                  = 0x8050,
  Rgb8                  = 0x8051,
  Rgb8SNorm             = 0x8F96,
  Rgb10                 = 0x8052,
  Rgb12                 = 0x8053,
  Rgb16SNorm            = 0x8F9A,
  Rgba2                 = 0x8055,
  Rgba4                 = 0x8056,
  Rgb5A1                = 0x8057,
  Rgba8                 = 0x8058,
  Rgba8SNorm            = 0x8F97,
  Rgb10A2               = 0x8059,
  Rgb10A2UI             = 0x906F,
  Rgba12                = 0x805A,
  Rgba16                = 0x805B,
  Srgb8                 = 0x8C41,
  Srgba8                = 0x8C43,
  R16F                  = 0x822D,
  Rg16F                 = 0x822F,
  Rgb16F                = 0x881B,
  Rgba16F               = 0x881A,
  R32F                  = 0x822E,
  Rg32F                 = 0x8230,
  Rgb32F                = 0x8815,
  Rgba32F               = 0x8814,
  Rg11B10F              = 0x8C3A,
  Rgb9E5                = 0x8C3D,
  R8I                   = 0x8231,
  R8UI                  = 0x8232,
  R16I                  = 0x8233,
  R16UI                 = 0x8234,
  R32I                  = 0x8235,
  R32UI                 = 0x8236,
  Rg8I                  = 0x8237,
  Rg8UI                 = 0x8238,
  Rg16I                 = 0x8239,
  Rg16UI                = 0x823A,
  Rg32I                 = 0x823B,
  Rg32UI                = 0x823C,
  Rgb8I                 = 0x8D8F,
  Rgb8UI                = 0x8D7D,
  Rgb16I                = 0x8D89,
  Rgb16UI               = 0x8D77,
  Rgb32I                = 0x8D83,
  Rgb32UI               = 0x8D71,
  Rgba8I                = 0x8D8E,
  Rgba8UI               = 0x8D7C,
  Rgba16I               = 0x8D88,
  Rgba16UI              = 0x8D76,
  Rgba32I               = 0x8D82,
  Rgba32UI              = 0x8D70,
  Depth16               = 0x81A5,
  Depth24               = 0x81A6,
  Depth32F              = 0x8CAC,
  Depth24Stencil8       = 0x88F0,
  Depth32FStencil8      = 0x8CAD
};

enum class OpenGLFormat
{
  ColorIndex            = 0x1900,
  StencilIndex          = 0x1901,
  DepthComponent        = 0x1902,
  Red                   = 0x1903,
  RedInteger            = 0x8D94,
  Green                 = 0x1904,
  GreenInteger          = 0x8D95,
  Blue                  = 0x1905,
  BlueInteger           = 0x8D96,
  Alpha                 = 0x1906,
  AlphaInteger          = 0x8D97,
  Rg                    = 0x8227,
  RgInteger             = 0x8228,
  Rgb                   = 0x1907,
  RgbInteger            = 0x8D98,
  Rgba                  = 0x1908,
  RgbaInteger           = 0x8D99,
  Luminance             = 0x1909,
  LuminanceAlpha        = 0x190A,
  DepthStencil          = 0x84F9
};

enum class OpenGLType
{
  Byte                      = 0x1400,
  UnsignedByte              = 0x1401,
  Short                     = 0x1402,
  UnsignedShort             = 0x1403,
  Int                       = 0x1404,
  UnsignedInt               = 0x1405,
  Float                     = 0x1406,
  HalfFloat                 = 0x140B,
  Double                    = 0x140A,
  UnsignedShort_5_6_5       = 0x8363,
  UnsignedInt_10F_11F_11F   = 0x8C3B,
  UnsignedInt_5_9_9_9_9     = 0x8C3E,
  UnsignedShort_4_4_4_4     = 0x8033,
  UnsignedShort_5_5_5_1     = 0x8034,
  UnsignedInt_2_10_10_10    = 0x8368,
  UnsignedInt_24_8          = 0x84FA,
  Float_32_UnsignedInt_24_8 = 0x8DAD
};

inline OpenGLFormat GetFormat(OpenGLInternalFormat s)
{
  switch (s)
  {
  case OpenGLInternalFormat::R8:
    return OpenGLFormat::Red;
  case OpenGLInternalFormat::R8SNorm:
    return OpenGLFormat::Red;
  case OpenGLInternalFormat::R16:
    return OpenGLFormat::Red;
  case OpenGLInternalFormat::R16SNorm:
    return OpenGLFormat::Red;
  case OpenGLInternalFormat::Rg8:
    return OpenGLFormat::Rg;
  case OpenGLInternalFormat::Rg8SNorm:
    return OpenGLFormat::Rg;
  case OpenGLInternalFormat::Rg16:
    return OpenGLFormat::Rg;
  case OpenGLInternalFormat::Rg16SNorm:
    return OpenGLFormat::Rg;
  case OpenGLInternalFormat::Rg3B2:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgb4:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgb5:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgb8:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgb8SNorm:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgb10:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgb12:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgb16SNorm:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgba2:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgba4:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgb5A1:
    return OpenGLFormat::Rgba;
  case OpenGLInternalFormat::Rgba8:
    return OpenGLFormat::Rgba;
  case OpenGLInternalFormat::Rgba8SNorm:
    return OpenGLFormat::Rgba;
  case OpenGLInternalFormat::Rgb10A2:
    return OpenGLFormat::Rgba;
  case OpenGLInternalFormat::Rgb10A2UI:
    return OpenGLFormat::RgbaInteger;
  case OpenGLInternalFormat::Rgba12:
    return OpenGLFormat::Rgba;
  case OpenGLInternalFormat::Rgba16:
    return OpenGLFormat::Rgba;
  case OpenGLInternalFormat::Srgb8:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Srgba8:
    return OpenGLFormat::Rgba;
  case OpenGLInternalFormat::R16F:
    return OpenGLFormat::Red;
  case OpenGLInternalFormat::Rg16F:
    return OpenGLFormat::Rg;
  case OpenGLInternalFormat::Rgb16F:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgba16F:
    return OpenGLFormat::Rgba;
  case OpenGLInternalFormat::R32F:
    return OpenGLFormat::Red;
  case OpenGLInternalFormat::Rg32F:
    return OpenGLFormat::Rg;
  case OpenGLInternalFormat::Rgb32F:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgba32F:
    return OpenGLFormat::Rgba;
  case OpenGLInternalFormat::Rg11B10F:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::Rgb9E5:
    return OpenGLFormat::Rgb;
  case OpenGLInternalFormat::R8I:
    return OpenGLFormat::RedInteger;
  case OpenGLInternalFormat::R8UI:
    return OpenGLFormat::RedInteger;
  case OpenGLInternalFormat::R16I:
    return OpenGLFormat::RedInteger;
  case OpenGLInternalFormat::R16UI:
    return OpenGLFormat::RedInteger;
  case OpenGLInternalFormat::R32I:
    return OpenGLFormat::RedInteger;
  case OpenGLInternalFormat::R32UI:
    return OpenGLFormat::RedInteger;
  case OpenGLInternalFormat::Rg8I:
    return OpenGLFormat::RgInteger;
  case OpenGLInternalFormat::Rg8UI:
    return OpenGLFormat::RgInteger;
  case OpenGLInternalFormat::Rg16I:
    return OpenGLFormat::RgInteger;
  case OpenGLInternalFormat::Rg16UI:
    return OpenGLFormat::RgInteger;
  case OpenGLInternalFormat::Rg32I:
    return OpenGLFormat::RgInteger;
  case OpenGLInternalFormat::Rg32UI:
    return OpenGLFormat::RgInteger;
  case OpenGLInternalFormat::Rgb8I:
    return OpenGLFormat::RgbInteger;
  case OpenGLInternalFormat::Rgb8UI:
    return OpenGLFormat::RgbInteger;
  case OpenGLInternalFormat::Rgb16I:
    return OpenGLFormat::RgbInteger;
  case OpenGLInternalFormat::Rgb16UI:
    return OpenGLFormat::RgbInteger;
  case OpenGLInternalFormat::Rgb32I:
    return OpenGLFormat::RgbInteger;
  case OpenGLInternalFormat::Rgb32UI:
    return OpenGLFormat::RgbInteger;
  case OpenGLInternalFormat::Rgba8I:
    return OpenGLFormat::RgbaInteger;
  case OpenGLInternalFormat::Rgba8UI:
    return OpenGLFormat::RgbaInteger;
  case OpenGLInternalFormat::Rgba16I:
    return OpenGLFormat::RgbaInteger;
  case OpenGLInternalFormat::Rgba16UI:
    return OpenGLFormat::RgbaInteger;
  case OpenGLInternalFormat::Rgba32I:
    return OpenGLFormat::RgbaInteger;
  case OpenGLInternalFormat::Rgba32UI:
    return OpenGLFormat::RgbInteger;
  case OpenGLInternalFormat::Depth16:
    return OpenGLFormat::DepthComponent;
  case OpenGLInternalFormat::Depth24:
    return OpenGLFormat::DepthComponent;
  case OpenGLInternalFormat::Depth32F:
    return OpenGLFormat::DepthComponent;
  case OpenGLInternalFormat::Depth24Stencil8:
    return OpenGLFormat::DepthStencil;
  case OpenGLInternalFormat::Depth32FStencil8:
    return OpenGLFormat::DepthStencil;
  }
  return OpenGLFormat(0);
}

inline OpenGLType GetType(OpenGLInternalFormat s)
{
  switch (s)
  {
  case OpenGLInternalFormat::R8:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::R8SNorm:
    return OpenGLType::Byte;
  case OpenGLInternalFormat::R16:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::R16SNorm:
    return OpenGLType::Byte;
  case OpenGLInternalFormat::Rg8:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rg8SNorm:
    return OpenGLType::Byte;
  case OpenGLInternalFormat::Rg16:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rg16SNorm:
    return OpenGLType::Byte;
  case OpenGLInternalFormat::Rg3B2:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgb4:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgb5:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgb8:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgb8SNorm:
    return OpenGLType::Byte;
  case OpenGLInternalFormat::Rgb10:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgb12:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgb16SNorm:
    return OpenGLType::Short;
  case OpenGLInternalFormat::Rgba2:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgba4:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgb5A1:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgba8:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgba8SNorm:
    return OpenGLType::Byte;
  case OpenGLInternalFormat::Rgb10A2:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgb10A2UI:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgba12:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgba16:
    return OpenGLType::UnsignedShort;
  case OpenGLInternalFormat::Srgb8:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Srgba8:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::R16F:
    return OpenGLType::HalfFloat;
  case OpenGLInternalFormat::Rg16F:
    return OpenGLType::HalfFloat;
  case OpenGLInternalFormat::Rgb16F:
    return OpenGLType::HalfFloat;
  case OpenGLInternalFormat::Rgba16F:
    return OpenGLType::HalfFloat;
  case OpenGLInternalFormat::R32F:
    return OpenGLType::Float;
  case OpenGLInternalFormat::Rg32F:
    return OpenGLType::Float;
  case OpenGLInternalFormat::Rgb32F:
    return OpenGLType::Float;
  case OpenGLInternalFormat::Rgba32F:
    return OpenGLType::Float;
  case OpenGLInternalFormat::Rg11B10F:
    return OpenGLType::Float;
  case OpenGLInternalFormat::Rgb9E5:
    return OpenGLType::Float;
  case OpenGLInternalFormat::R8I:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::R8UI:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::R16I:
    return OpenGLType::Short;
  case OpenGLInternalFormat::R16UI:
    return OpenGLType::UnsignedShort;
  case OpenGLInternalFormat::R32I:
    return OpenGLType::Int;
  case OpenGLInternalFormat::R32UI:
    return OpenGLType::UnsignedInt;
  case OpenGLInternalFormat::Rg8I:
    return OpenGLType::Byte;
  case OpenGLInternalFormat::Rg8UI:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rg16I:
    return OpenGLType::Short;
  case OpenGLInternalFormat::Rg16UI:
    return OpenGLType::UnsignedShort;
  case OpenGLInternalFormat::Rg32I:
    return OpenGLType::Int;
  case OpenGLInternalFormat::Rg32UI:
    return OpenGLType::UnsignedInt;
  case OpenGLInternalFormat::Rgb8I:
    return OpenGLType::Byte;
  case OpenGLInternalFormat::Rgb8UI:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgb16I:
    return OpenGLType::Short;
  case OpenGLInternalFormat::Rgb16UI:
    return OpenGLType::UnsignedShort;
  case OpenGLInternalFormat::Rgb32I:
    return OpenGLType::Int;
  case OpenGLInternalFormat::Rgb32UI:
    return OpenGLType::UnsignedInt;
  case OpenGLInternalFormat::Rgba8I:
    return OpenGLType::Byte;
  case OpenGLInternalFormat::Rgba8UI:
    return OpenGLType::UnsignedByte;
  case OpenGLInternalFormat::Rgba16I:
    return OpenGLType::Short;
  case OpenGLInternalFormat::Rgba16UI:
    return OpenGLType::UnsignedShort;
  case OpenGLInternalFormat::Rgba32I:
    return OpenGLType::Int;
  case OpenGLInternalFormat::Rgba32UI:
    return OpenGLType::UnsignedInt;
  case OpenGLInternalFormat::Depth16:
    return OpenGLType::UnsignedShort;
  case OpenGLInternalFormat::Depth24:
    return OpenGLType::UnsignedInt;
  case OpenGLInternalFormat::Depth32F:
    return OpenGLType::Float;
  case OpenGLInternalFormat::Depth24Stencil8:
    return OpenGLType::UnsignedInt_24_8;
  case OpenGLInternalFormat::Depth32FStencil8:
    return OpenGLType::Float_32_UnsignedInt_24_8;
  }
  return OpenGLType(0);
}

#endif // OPENGLSTORAGE_H
