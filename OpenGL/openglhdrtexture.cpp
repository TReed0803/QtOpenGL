#include "openglhdrtexture.h"

#include <KMacros>
#include <KMath>
#include <OpenGLTexture>
#include <OpenGLToneMappingFunction>

class OpenGLHdrTextureLoaderPrivate
{
public:
  OpenGLHdrTextureLoaderPrivate(OpenGLTexture *texture);
  OpenGLTexture *m_texture;
  int m_width, m_height;
  std::vector<float> m_textureData;
  std::vector<float> m_lodData;
  OpenGLToneMappingFunction *m_toneMapping;
};

OpenGLHdrTextureLoaderPrivate::OpenGLHdrTextureLoaderPrivate(OpenGLTexture *texture) :
  m_texture(texture)
{
  // Intentionally Empty
}

OpenGLHdrTextureLoader::OpenGLHdrTextureLoader(KAbstractReader *reader, OpenGLTexture *texture) :
  KAbstractHdrParser(reader), m_private(new OpenGLHdrTextureLoaderPrivate(texture))
{
  // Intentionally Empty
}

bool OpenGLHdrTextureLoader::parse(OpenGLToneMappingFunction *toneMap)
{
  P(OpenGLHdrTextureLoaderPrivate);
  p.m_toneMapping = toneMap;
  return KAbstractHdrParser::parse();
}

void OpenGLHdrTextureLoader::onKeyValue(const char *key, const char *value)
{
  // Handle key/value pairs here
}

void OpenGLHdrTextureLoader::onResolution(PixelOrder xOrder, PixelOrder yOrder, int width, int height)
{
  P(OpenGLHdrTextureLoaderPrivate);
  (void)xOrder;
  (void)yOrder;
  p.m_width = width;
  p.m_height = height;
}

float *OpenGLHdrTextureLoader::beginData()
{
  P(OpenGLHdrTextureLoaderPrivate);
  p.m_textureData.resize(3 * p.m_width * p.m_height);
  return p.m_textureData.data();
}

void OpenGLHdrTextureLoader::endData()
{
  P(OpenGLHdrTextureLoaderPrivate);

  // Apply Tone Mapping
  for (int j=0;  j<p.m_height;  j++)
  {
    int jk = (j*p.m_width*3);
    for (int i=0;  i<p.m_width; i++)
    {
      RgbF &color = reinterpret_cast<RgbF&>(p.m_textureData[jk + i * 3]);
      color = (*p.m_toneMapping)(color);
      color = std::pow(color, 2.2f); // Gamma Correction
    }
  }

  // Create the textures
  p.m_texture->create(OpenGLTexture::Texture2D);
  p.m_texture->bind();
  p.m_texture->setInternalFormat(OpenGLInternalFormat::Rgb32F);
  p.m_texture->setWrapMode(OpenGLTexture::DirectionS, OpenGLTexture::ClampToEdge);
  p.m_texture->setWrapMode(OpenGLTexture::DirectionT, OpenGLTexture::ClampToEdge);
  p.m_texture->setFilter(OpenGLTexture::Magnification, OpenGLTexture::NearestMipMap);
  p.m_texture->setFilter(OpenGLTexture::Minification, OpenGLTexture::NearestMipMap);
  p.m_texture->setSize(p.m_width, p.m_height);
  p.m_texture->setSwizzle(OpenGLTexture::Red, OpenGLTexture::Green, OpenGLTexture::Blue, OpenGLTexture::One);
  p.m_texture->allocate(p.m_textureData.data(), 0);
  p.m_texture->generateMipMaps();
  p.m_texture->getMaxLevel();
  p.m_texture->release();
}
