#include "openglspotlight.h"

#include <KMath>

OpenGLSpotLight::OpenGLSpotLight() :
  m_depth(20.0f)
{
  setInnerAngle(15.0f);
  setOuterAngle(30.0f);
}

float OpenGLSpotLight::CalculateScalar(int segments)
{
  float segArcLength = Karma::Pi / segments;
  float yAxisLength  = Karma::sec(segArcLength);
  return std::abs(yAxisLength);
}

void OpenGLSpotLight::setInnerAngle(float angle)
{
  angle /= 2.0f;
  float rads = Karma::DegreesToRads(angle);
  m_innerAngle = std::cos(rads);
}

void OpenGLSpotLight::setOuterAngle(float angle)
{
  angle /= 2.0f;
  float rads = Karma::DegreesToRads(angle);
  m_outerAngle = std::cos(rads);
  m_angleOfInfluence = rads;
}

void OpenGLSpotLight::setDepth(float d)
{
  float dz = d * std::tan(m_angleOfInfluence) * CalculateScalar(32);
  m_depth = d;
  m_transform.setScaleX(dz);
  m_transform.setScaleY(dz);
  m_transform.setScaleZ(d);
}

