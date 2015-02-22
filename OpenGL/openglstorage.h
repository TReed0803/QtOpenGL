#ifndef OPENGLSTORAGE_H
#define OPENGLSTORAGE_H OpenGLStorage
#include <QtOpenGL/QGL>

enum class OpenGLStorage
{
  // Pixel Format
  Red                          = 0x1903,
  Green                        = 0x1904,
  Blue                         = 0x1905,
  Alpha                        = 0x1906,
  Rgb                          = 0x1907,
  Rgba                         = 0x1908,
  Luminance                    = 0x1909,
  LuminanceAlpha               = 0x190A,

  // Misc.
  R8                           = 0x8229,
  R8I                          = 0x8231,
  R8UI                         = 0x8232,
  R16                          = 0x822A,
  R16I                         = 0x8233,
  R16UI                        = 0x8234,
  R16F                         = 0x822D,
  R32I                         = 0x8235,
  R32UI                        = 0x8236,
  R32F                         = 0x822E,
  Rg8                          = 0x822B,
  Rg8I                         = 0x8237,
  Rg8UI                        = 0x8238,
  Rg16                         = 0x822C,
  Rg16I                        = 0x8239,
  Rg16UI                       = 0x823A,
  Rg16F                        = 0x822F,
  Rg32I                        = 0x823B,
  Rg32UI                       = 0x823C,
  Rg32F                        = 0x8230,
  Rgb4                         = 0x804F,
  Rgb5                         = 0x8050,
  Rgb8                         = 0x8051,
  Rgb10                        = 0x8052,
  Rgb12                        = 0x8053,
  Rgb16                        = 0x8054,
  Rgba2                        = 0x8055,
  Rgba4                        = 0x8056,
  Rgb5_A1                      = 0x8057,
  Rgba8                        = 0x8058,
  Rgb10_A2                     = 0x8059,
  Rgba12                       = 0x805A,
  Rgba16                       = 0x805B,
  Srgb8                        = 0x8C41,
  Srgba8                       = 0x8C43,
  Depth16                      = 0x81A5,
  Depth24                      = 0x81A6,
  Depth32                      = 0x81A7,
  Stencil1                     = 0x8D46,
  Stencil4                     = 0x8D47,
  Stencil8                     = 0x8D48,
  Stencil16                    = 0x8D49,
  Deptch24_Stencil8            = 0x88F0,
  Deptch32f_Stencil8           = 0x8CAD
};

#endif // OPENGLSTORAGE_H
