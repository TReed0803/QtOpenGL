#include "openglpointlight.h"

#include <KMath>

OpenGLPointLight::OpenGLPointLight() :
  m_radius(20.0f)
{
  // Intentionally Empty
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

void OpenGLPointLight::setRadius(float r)
{
  m_radius = r;
  m_transform.setScale(r * OpenGLPointLight::CalculateScalar(12, 8));
}

