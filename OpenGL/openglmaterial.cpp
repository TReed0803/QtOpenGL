#include "openglmaterial.h"

#include <Kmath>
#include <KColor>
#include <KMacros>
#include <KVector2D>
#include <OpenGLUniformBufferObject>
#include <OpenGLBindings>
#include <OpenGLMaterialData>

/*******************************************************************************
 * OpenGLMaterialPrivate
 ******************************************************************************/
class OpenGLMaterialPrivate
{
public:
  KColor m_diffuse;
  KColor m_fresnel;
  KVector2D m_roughness;
  OpenGLUniformBufferObject m_buffer;
};

/*******************************************************************************
 * OpenGLMaterial
 ******************************************************************************/
OpenGLMaterial::OpenGLMaterial() :
  m_private(new OpenGLMaterialPrivate)
{
  // Intentionally Empty
}

OpenGLMaterial::~OpenGLMaterial()
{
  // Intentionally Empty
}

void OpenGLMaterial::create()
{
  P(OpenGLMaterialPrivate);
  p.m_buffer.create();
  p.m_buffer.bind();
  p.m_buffer.allocate(sizeof(OpenGLMaterialData));
  p.m_buffer.release();
}

void OpenGLMaterial::bind()
{
  P(OpenGLMaterialPrivate);
  p.m_buffer.bindBase(K_MATERIAL_BINDING);
}

void OpenGLMaterial::commit()
{
  P(OpenGLMaterialPrivate);
  OpenGLBuffer::RangeAccessFlags flags =
    OpenGLBuffer::RangeUnsynchronized   |
    OpenGLBuffer::RangeInvalidateBuffer |
    OpenGLBuffer::RangeWrite;

  p.m_buffer.bind();

  // Send data to the GPU
  {
    OpenGLMaterialData *data = (OpenGLMaterialData*)p.m_buffer.mapRange(0, sizeof(OpenGLMaterialData), flags);
    data->m_diffuse = Karma::ToGlm(p.m_diffuse);
    data->m_fresnel = Karma::ToGlm(p.m_fresnel);
    data->m_roughness = Karma::ToGlm(p.m_roughness);
    p.m_buffer.unmap();
  }

  p.m_buffer.release();
}

void OpenGLMaterial::release()
{
  OpenGLUniformBufferObject::bindBufferId(K_MATERIAL_BINDING, 0);
}

int OpenGLMaterial::objectId() const
{
  P(const OpenGLMaterialPrivate);
  return p.m_buffer.bufferId();
}

///////////////////////////////////////////////////////////////////////////////

void OpenGLMaterial::setDiffuse(float rgb)
{
  P(OpenGLMaterialPrivate);
  p.m_diffuse.setRgb(rgb, rgb, rgb);
}

void OpenGLMaterial::setDiffuse(char r, char g, char b)
{
  P(OpenGLMaterialPrivate);
  p.m_diffuse.setRgb(r, g, b);
}

void OpenGLMaterial::setDiffuse(float r, float g, float b)
{
  P(OpenGLMaterialPrivate);
  p.m_diffuse.setRgbF(r, g, b);
}

void OpenGLMaterial::setDiffuse(const KColor &color)
{
  P(OpenGLMaterialPrivate);
  p.m_diffuse = color;
}

const KColor &OpenGLMaterial::diffuse() const
{
  P(const OpenGLMaterialPrivate);
  return p.m_diffuse;
}

///////////////////////////////////////////////////////////////////////////////

void OpenGLMaterial::setFresnel(float rgb)
{
  P(OpenGLMaterialPrivate);
  p.m_fresnel.setRgbF(rgb, rgb, rgb);
}

void OpenGLMaterial::setFresnel(char r, char g, char b)
{
  P(OpenGLMaterialPrivate);
  p.m_fresnel.setRgb(r, g, b);
}

void OpenGLMaterial::setFresnel(float r, float g, float b)
{
  P(OpenGLMaterialPrivate);
  p.m_fresnel.setRgbF(r, g, b);
}

void OpenGLMaterial::setFresnel(const KColor &color)
{
  P(OpenGLMaterialPrivate);
  p.m_fresnel = color;
}

const KColor &OpenGLMaterial::color() const
{
  P(const OpenGLMaterialPrivate);
  return p.m_fresnel;
}

///////////////////////////////////////////////////////////////////////////////

void OpenGLMaterial::setRoughness(float xy)
{
  P(OpenGLMaterialPrivate);
  xy = xy * xy;
  p.m_roughness = KVector2D(xy, xy);
}

void OpenGLMaterial::setRoughness(float x, float y)
{
  P(OpenGLMaterialPrivate);
  p.m_roughness = KVector2D(x * x, y * y);
}

const KVector2D &OpenGLMaterial::roughness() const
{
  P(const OpenGLMaterialPrivate);
  return p.m_roughness;
}

