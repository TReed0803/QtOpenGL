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
  KVector3D m_baseColor;
  float m_metallic;
  float m_roughness;
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
    static const float MinValue = 1.0e-2f;
    OpenGLMaterialData *data = (OpenGLMaterialData*)p.m_buffer.mapRange(0, sizeof(OpenGLMaterialData), flags);
    data->m_baseColor = Karma::ToGlm(p.m_baseColor);
    if (glm::length(data->m_baseColor) <= MinValue) data->m_baseColor = Karma::ToGlm(MinValue, MinValue, MinValue);
    data->m_baseColor = glm::pow(data->m_baseColor, glm::vec3(2.2f));
    data->m_metallic = p.m_metallic;
    data->m_roughness = p.m_roughness * p.m_roughness * p.m_roughness;
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

void OpenGLMaterial::setBaseColor(float rgb)
{
  P(OpenGLMaterialPrivate);
  p.m_baseColor = KVector3D(rgb, rgb, rgb);
}

void OpenGLMaterial::setBaseColor(float r, float g, float b)
{
  P(OpenGLMaterialPrivate);
  p.m_baseColor = KVector3D(r, g, b);
}

void OpenGLMaterial::setBaseColor(const KVector3D &color)
{
  P(OpenGLMaterialPrivate);
  p.m_baseColor = color;
}

const KVector3D &OpenGLMaterial::baseColor() const
{
  P(const OpenGLMaterialPrivate);
  return p.m_baseColor;
}

///////////////////////////////////////////////////////////////////////////////

// Notes on Metallic Property:
// Metallic is basically a direct correlation to F0 fresnel term.
//
// Dielectrics (Poor conduction materials)
// --> Total Range: [0.02, 0.2]
// --> Water/Rubber (0.02)
// --> Plastic, Glass (0.04~0.045)
// --> Crystalware, Gems (0.05~0.08)
// --> Diamond-Like (0.1~0.2)
//
// Semiconductors (Conductivity between most metals/insulator)
// --> Total Range: [0.2, 0.5]
// --> Crystalline Silicon (0.345, 0.369, 0.426)
//
// Metals (Completly conductive materials)
// --> Total Range: [0.5, 1.0]
// --> Titanium (~0.496)
// --> Chromium (~0.553)
// --> Iron (~0.568)
// --> Nickel (~0.598)
// --> Platinum (~0.631)
// --> Palladium (~0.694)
// --> Copper (~0.710)
// --> Gold (~0.716)
// --> Zinc (~0.779)
// --> Aluminum (~0.919)
// --> Silver (~0.949)
//
// Because of this physical range, we will tend to clamp the
// possible reflective values between [0.02~0.95]. In this case
// I chose to clamp between [0.04~0.95] since I will not be modelling
// water physically. But it might be worth playing around with.
void OpenGLMaterial::setMetallic(float m)
{
  P(OpenGLMaterialPrivate);
  p.m_metallic = Karma::clamp(m, 0.0, 1.0);
}

float OpenGLMaterial::metallic() const
{
  P(const OpenGLMaterialPrivate);
  return p.m_metallic;
}

///////////////////////////////////////////////////////////////////////////////

void OpenGLMaterial::setRoughness(float r)
{
  P(OpenGLMaterialPrivate);
  p.m_roughness = Karma::clamp(r, 0.0f, 1.0f);
}

float OpenGLMaterial::roughness() const
{
  P(const OpenGLMaterialPrivate);
  return p.m_roughness;
}
