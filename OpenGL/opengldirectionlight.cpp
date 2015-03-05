#include "opengldirectionlight.h"

#include <KColor>
#include <KMacros>
#include <KVector3D>

class OpenGLDirectionLightPrivate
{
public:
  OpenGLDirectionLightPrivate();
  KVector3D m_direction;
  KColor m_diffuse;
  KColor m_specular;
};

OpenGLDirectionLightPrivate::OpenGLDirectionLightPrivate() :
  m_direction(-0.57735f, -0.57735f, -0.57735f), m_diffuse(0.8f, 0.8f, 0.8f), m_specular(1.0f, 1.0f, 1.0f)
{
  // Intentionally Empty
}

OpenGLDirectionLight::OpenGLDirectionLight() :
  m_private(new OpenGLDirectionLightPrivate)
{
  // Intentionally Empty
}

OpenGLDirectionLight::~OpenGLDirectionLight()
{
  // Intentionally Empty
}

void OpenGLDirectionLight::setDirection(float x, float y, float z)
{
  P(OpenGLDirectionLightPrivate);
  p.m_direction = KVector3D(x, y, z);
  p.m_direction.normalize();
}

void OpenGLDirectionLight::setDirection(const KVector3D &dir)
{
  P(OpenGLDirectionLightPrivate);
  p.m_direction = dir;
  p.m_direction.normalize();
}

const KVector3D &OpenGLDirectionLight::direction() const
{
  P(const OpenGLDirectionLightPrivate);
  return p.m_direction;
}

void OpenGLDirectionLight::setDiffuse(float r, float g, float b)
{
  P(OpenGLDirectionLightPrivate);
  p.m_diffuse = KColor(r, g, b);
}

void OpenGLDirectionLight::setDiffuse(const KColor &rgb)
{
  P(OpenGLDirectionLightPrivate);
  p.m_diffuse = rgb;
}

const KColor &OpenGLDirectionLight::diffuse() const
{
  P(const OpenGLDirectionLightPrivate);
  return p.m_diffuse;
}

void OpenGLDirectionLight::setSpecular(float r, float g, float b)
{
  P(OpenGLDirectionLightPrivate);
  p.m_specular = KColor(r, g, b);
}

void OpenGLDirectionLight::setSpecular(const KColor &rgb)
{
  P(OpenGLDirectionLightPrivate);
  p.m_specular = rgb;
}

const KColor &OpenGLDirectionLight::specular()
{
  P(OpenGLDirectionLightPrivate);
  return p.m_specular;
}
