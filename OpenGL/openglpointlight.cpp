#include "openglpointlight.h"

#include <KColor>
#include <KVector3D>
#include <KMath>
#include <KTransform3D>
#include <KMacros>

class OpenGLPointLightPrivate
{
public:
  OpenGLPointLightPrivate();
  KTransform3D m_transform;
  KVector3D m_attenuation;
  float m_radius;
  KColor m_ambient;
  KColor m_diffuse;
  KColor m_specular;
};

OpenGLPointLightPrivate::OpenGLPointLightPrivate() :
  m_diffuse(0.8f, 0.8f, 0.8f), m_specular(1.0f, 1.0f, 1.0f), m_radius(20.0f), m_attenuation(1.0f, 0.01f, 0.1f)
{
  // Intentionally Empty
}


OpenGLPointLight::OpenGLPointLight() :
  m_private(new OpenGLPointLightPrivate)
{
  // Intentionally Empty
}

OpenGLPointLight::~OpenGLPointLight()
{
  delete m_private;
}

float OpenGLPointLight::CalculateScalar(int segments, int rings)
{
  if (rings % 2)

    // Even Number of rings
    return
      std::sqrt(
        std::pow(Karma::sec(Karma::Pi / segments ), 2.0f)
      * std::pow(Karma::sec(Karma::Pi / (2*rings)), 2.0f)
      );

  // Odd number of rings
  return
    Karma::Sqrt2
    * std::sqrt
      (
        1.0f /
        (
          1.0f
          + std::cos(Karma::TwoPi / segments)
          * std::cos(Karma::Pi    /    rings)
        )
      );
}

void OpenGLPointLight::translate(float x, float y, float z)
{
  P(OpenGLPointLightPrivate);
  p.m_transform.translate(x, y, z);
}

void OpenGLPointLight::translate(const KVector3D &trans)
{
  P(OpenGLPointLightPrivate);
  p.m_transform.translate(trans);
}

void OpenGLPointLight::setTranslation(float x, float y, float z)
{
  P(OpenGLPointLightPrivate);
  p.m_transform.setTranslation(x, y, z);
}

void OpenGLPointLight::setTranslation(const KVector3D &pos)
{
  P(OpenGLPointLightPrivate);
  p.m_transform.setTranslation(pos);
}

const KVector3D &OpenGLPointLight::translation() const
{
  P(OpenGLPointLightPrivate);
  return p.m_transform.translation();
}

void OpenGLPointLight::setAttenuation(float c, float ex1, float ex2)
{
  P(OpenGLPointLightPrivate);
  p.m_attenuation = KVector3D(c, ex1, ex2);
}

void OpenGLPointLight::setAttenuation(const KVector3D &attn)
{
  P(OpenGLPointLightPrivate);
  p.m_attenuation = attn;
}

const KVector3D &OpenGLPointLight::attenuation() const
{
  P(OpenGLPointLightPrivate);
  return p.m_attenuation;
}

void OpenGLPointLight::setAmbient(float r, float g, float b)
{
  P(OpenGLPointLightPrivate);
  p.m_ambient = KColor(r, g, b);
}

void OpenGLPointLight::setAmbient(const KColor &rgb)
{
  P(OpenGLPointLightPrivate);
  p.m_ambient = rgb;
}

const KColor &OpenGLPointLight::ambient() const
{
  P(OpenGLPointLightPrivate);
  return p.m_ambient;
}

void OpenGLPointLight::setDiffuse(float r, float g, float b)
{
  P(OpenGLPointLightPrivate);
  p.m_diffuse = KColor(r, g, b);
}

void OpenGLPointLight::setDiffuse(const KColor &rgb)
{
  P(OpenGLPointLightPrivate);
  p.m_diffuse = rgb;
}

const KColor &OpenGLPointLight::diffuse() const
{
  P(OpenGLPointLightPrivate);
  return p.m_diffuse;
}

void OpenGLPointLight::setSpecular(float r, float g, float b)
{
  P(OpenGLPointLightPrivate);
  p.m_specular = KColor(r, g, b);
}

void OpenGLPointLight::setSpecular(const KColor &rgb)
{
  P(OpenGLPointLightPrivate);
  p.m_specular = rgb;
}

const KColor &OpenGLPointLight::specular()
{
  P(OpenGLPointLightPrivate);
  return p.m_specular;
}

float OpenGLPointLight::radius() const
{
  P(OpenGLPointLightPrivate);
  return p.m_radius;
}

void OpenGLPointLight::setRadius(float r)
{
  P(OpenGLPointLightPrivate);
  p.m_radius = r;
  p.m_transform.setScale(r * OpenGLPointLight::CalculateScalar(12, 8));
}


const KMatrix4x4 &OpenGLPointLight::toMatrix()
{
  P(OpenGLPointLightPrivate);
  return p.m_transform.toMatrix();
}
