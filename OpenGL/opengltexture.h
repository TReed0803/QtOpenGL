#ifndef OPENGLTEXTURE_H
#define OPENGLTEXTURE_H OpenGLTexture

#include <OpenGLStorage>

class KSize;

class OpenGLTexturePrivate;
class OpenGLTexture
{
public:

  enum Target
  {
    Texture1D                   = 0x0DE0,
    Texture2D                   = 0x0DE1,
    TextureRectangle            = 0x84F5,
    TextureCubeMap              = 0x8513,
    ProxyTexture1D              = 0x8063,
    ProxyTexture2D              = 0x8064,
    ProxyTextureRectangle       = 0x84F7,
    ProxyTextureCubeMap         = 0x851B
  };

  enum Direction
  {
    DirectionS                  = 0x2802,
    DirectionT                  = 0x2803,
  };
  enum WrapMode
  {
    Clamp                       = 0x2900,
    Repeat                      = 0x2901,
    ClampToEdge                 = 0x812F,
    MirroredRepeat              = 0x8370
  };

  enum FilterType
  {
    Magnification               = 0x2800,
    Minification                = 0x2801
  };

  enum FilterOperation
  {
    Nearest                     = 0x2600,
    Linear                      = 0x2601
  };

  OpenGLTexture();
  ~OpenGLTexture();
  void create(Target type);
  void bind();
  void release();
  void setWrapMode(Direction dir, WrapMode mode);
  void setFilter(FilterType filter, FilterOperation op);
  void setSize(int width, int height);
  void setSize(KSize const &size);
  void setInternalFormat(OpenGLInternalFormat f);
  void allocate();
  int textureId();
  Target target() const;

private:
  OpenGLTexturePrivate *m_private;
  void removeReference();
};

#endif // OPENGLTEXTURE_H
