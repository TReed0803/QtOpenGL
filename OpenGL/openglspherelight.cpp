#include "openglspherelight.h"

OpenGLSphereLight::OpenGLSphereLight() :
  m_radius(0.5f)
{
  // Intentionally Empty
}

void OpenGLSphereLight::setRadius(float r)
{
  m_radius = r;
}

float OpenGLSphereLight::radius() const
{
  return m_radius;
}
