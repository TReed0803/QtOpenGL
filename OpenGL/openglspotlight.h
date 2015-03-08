#ifndef OPENGLSPOTLIGHT_H
#define OPENGLSPOTLIGHT_H OpenGLSpotLight

#include <OpenGLTranslationLight>

class OpenGLSpotLightPrivate;
class OpenGLSpotLight : public OpenGLTranslationLight
{
public:
  OpenGLSpotLight();

  // Static Scalar
  static float CalculateScalar(int segments);

  // Spotlight Settings
  void setInnerAngle(float angle);
  float innerAngle() const;
  void setOuterAngle(float angle);
  float outerAngle() const;
  void setDepth(float d);
  float depth() const;

private:
  float m_depth;
  float m_innerAngle;
  float m_outerAngle;
  float m_angleOfInfluence;
};

inline float OpenGLSpotLight::innerAngle() const
{
  return m_innerAngle;
}

inline float OpenGLSpotLight::outerAngle() const
{
  return m_outerAngle;
}

inline float OpenGLSpotLight::depth() const
{
  return m_depth;
}

#endif // OPENGLSPOTLIGHT_H
