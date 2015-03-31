#include "opengltexture.h"

#include <KSize>
#include <OpenGLFunctions>

class OpenGLTexturePrivate
{
public:
  int m_references;
  OpenGLTexture::Target m_target;
  GLuint m_textureId;
  KSize m_size;
  OpenGLInternalFormat m_format;
  OpenGLTexturePrivate(OpenGLTexture::Target t);
  ~OpenGLTexturePrivate();
};

OpenGLTexturePrivate::OpenGLTexturePrivate(OpenGLTexture::Target t) :
  m_references(1), m_target(t), m_textureId(0), m_size(1, 1), m_format(OpenGLInternalFormat::Rgba8)
{
  GL::glGenTextures(1, &m_textureId);
}

OpenGLTexturePrivate::~OpenGLTexturePrivate()
{
  GL::glDeleteTextures(1, &m_textureId);
}


OpenGLTexture::OpenGLTexture() :
  m_private(0)
{
  // Intentionally Empty
}

OpenGLTexture::~OpenGLTexture()
{
  removeReference();
}

void OpenGLTexture::create(OpenGLTexture::Target type)
{
  removeReference();
  m_private = new OpenGLTexturePrivate(type);
}

void OpenGLTexture::bind()
{
  P(OpenGLTexturePrivate);
  GL::glBindTexture(p.m_target, p.m_textureId);
}

void OpenGLTexture::release()
{
  P(OpenGLTexturePrivate);
  GL::glBindTexture(p.m_target, 0);
}

void OpenGLTexture::setSwizzle(OpenGLTexture::SwizzleMode r, OpenGLTexture::SwizzleMode g, OpenGLTexture::SwizzleMode b, OpenGLTexture::SwizzleMode a)
{
  P(OpenGLTexturePrivate);
  GLint swizzleMask[] = { r, g, b, a };
  GL::glTexParameteriv(p.m_target, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
}

void OpenGLTexture::setWrapMode(OpenGLTexture::Direction dir, OpenGLTexture::WrapMode mode)
{
  P(OpenGLTexturePrivate);
  GL::glTexParameteri(p.m_target, dir, mode);
}

void OpenGLTexture::setFilter(OpenGLTexture::FilterType filter, OpenGLTexture::FilterOperation op)
{
  P(OpenGLTexturePrivate);
  GL::glTexParameteri(p.m_target, filter, op);
}

void OpenGLTexture::setSize(int width, int height)
{
  P(OpenGLTexturePrivate);
  p.m_size.setWidth(width);
  p.m_size.setHeight(height);
}

void OpenGLTexture::setSize(const KSize &size)
{
  setSize(size.width(), size.height());
}

void OpenGLTexture::setInternalFormat(OpenGLInternalFormat f)
{
  P(OpenGLTexturePrivate);
  p.m_format = f;
}

void OpenGLTexture::setCompareMode(OpenGLTexture::CompareMode mode)
{
  P(OpenGLTexturePrivate);
  GL::glTexParameteri(p.m_target, GL_TEXTURE_COMPARE_MODE, mode);
}

void OpenGLTexture::setCompareFunction(OpenGLTexture::CompareFunction func)
{
  P(OpenGLTexturePrivate);
  GL::glTexParameteri(p.m_target, GL_TEXTURE_COMPARE_FUNC, func);
}

void OpenGLTexture::allocate()
{
  allocate(0);
}

void OpenGLTexture::allocate(void *data)
{
  P(OpenGLTexturePrivate);
  switch (p.m_target)
  {
  case Texture2D:
    GL::glTexImage2D(p.m_target, 0, static_cast<GLint>(p.m_format), p.m_size.width(), p.m_size.height(), 0, static_cast<GLenum>(GetFormat(p.m_format)), static_cast<GLenum>(GetType(p.m_format)), (GLvoid*)data);
    break;
  case Texture1D:
  case TextureRectangle:
  case TextureCubeMap:
  case ProxyTexture1D:
  case ProxyTexture2D:
  case ProxyTextureRectangle:
  case ProxyTextureCubeMap:
    qFatal("Unsupported Texture Type");
    break;
  }
}

int OpenGLTexture::textureId()
{
  P(OpenGLTexturePrivate);
  return p.m_textureId;
}

OpenGLTexture::Target OpenGLTexture::target() const
{
  P(OpenGLTexturePrivate);
  return p.m_target;
}

int OpenGLTexture::numTextureUnits()
{
  return GL::getInteger<GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS>();
}

GLenum OpenGLTexture::beginTextureUnits()
{
  return GL_TEXTURE0;
}

GLenum OpenGLTexture::endTextureUnits()
{
  return GL_TEXTURE0 + numTextureUnits();
}

void OpenGLTexture::removeReference()
{
  if (m_private)
  {
    P(OpenGLTexturePrivate);
    --p.m_references;
    if (p.m_references == 0)
    {
      delete m_private;
      m_private = 0;
    }
  }
}


