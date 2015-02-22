#include "opengltexture.h"

#include <KSize>
#include <OpenGLFunctions>

class OpenGLTexturePrivate
{
public:
  int m_references;
  OpenGLFunctions m_functions;
  OpenGLTexture::Target m_target;
  GLuint m_textureId;
  KSize m_size;
  OpenGLStorage m_storage;
  OpenGLTexturePrivate(OpenGLTexture::Target t);
  ~OpenGLTexturePrivate();
};

OpenGLTexturePrivate::OpenGLTexturePrivate(OpenGLTexture::Target t) :
  m_references(1), m_target(t), m_textureId(0), m_size(1, 1), m_storage(OpenGLStorage::Rgba)
{
  m_functions.initializeOpenGLFunctions();
  m_functions.glGenTextures(1, &m_textureId);
}

OpenGLTexturePrivate::~OpenGLTexturePrivate()
{
  m_functions.glDeleteTextures(1, &m_textureId);
}

#define P(c) c &p = *m_private
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
  p.m_functions.glBindTexture(p.m_target, p.m_textureId);
}

void OpenGLTexture::release()
{
  P(OpenGLTexturePrivate);
  p.m_functions.glBindTexture(p.m_target, 0);
}

void OpenGLTexture::setWrapMode(OpenGLTexture::Direction dir, OpenGLTexture::WrapMode mode)
{
  P(OpenGLTexturePrivate);
  p.m_functions.glTexParameteri(p.m_target, dir, mode);
}

void OpenGLTexture::setFilter(OpenGLTexture::FilterType filter, OpenGLTexture::FilterOperation op)
{
  P(OpenGLTexturePrivate);
  p.m_functions.glTexParameteri(p.m_target, filter, op);
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

void OpenGLTexture::setStorage(OpenGLStorage s)
{
  P(OpenGLTexturePrivate);
  p.m_storage = s;
}

void OpenGLTexture::allocate()
{
  P(OpenGLTexturePrivate);
  switch (p.m_target)
  {
  case Texture2D:
    p.m_functions.glTexImage2D(p.m_target, 0, GL_RGBA32F, p.m_size.width(), p.m_size.height(), 0, GL_RGBA, GL_FLOAT, (GLvoid*)0);
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


