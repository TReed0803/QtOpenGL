#include "openglrectanglelight.h"

OpenGLRectangleLight::OpenGLRectangleLight() :
  m_halfWidth(0.5f), m_halfHeight(0.5f)
{
  // Intentionally Empty
}

void OpenGLRectangleLight::setWidth(float w)
{
  m_halfWidth = w / 2.0f;
}

void OpenGLRectangleLight::setHalfWidth(float hw)
{
  m_halfWidth = hw;
}

void OpenGLRectangleLight::setHeight(float h)
{
  m_halfHeight = h / 2.0f;
}

void OpenGLRectangleLight::setHalfHeight(float hh)
{
  m_halfHeight = hh;
}

void OpenGLRectangleLight::setDimensions(float w, float h)
{
  m_halfWidth = w / 2.0f;
  m_halfHeight = h / 2.0f;
}

void OpenGLRectangleLight::setHalfDimensions(float hw, float hh)
{
  m_halfWidth = hw;
  m_halfHeight = hh;
}

float OpenGLRectangleLight::width() const
{
  return 2.0 * m_halfWidth;
}

float OpenGLRectangleLight::halfWidth() const
{
  return m_halfWidth;
}

float OpenGLRectangleLight::height() const
{
  return 2.0 * m_halfHeight;
}

float OpenGLRectangleLight::halfHeight() const
{
  return m_halfHeight;
}
