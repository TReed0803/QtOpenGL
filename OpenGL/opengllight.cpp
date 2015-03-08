#include "opengllight.h"

OpenGLLight::OpenGLLight() :
  m_shadowCasting(false)
, m_attenuation(1.0f, 0.01f, 0.1f)
, m_diffuse(0.8f, 0.8f, 0.8f)
, m_specular(1.0f, 1.0f, 1.0f)
{
  // Intentionally Empty
}
