#ifndef   OPENGLPOINTLIGHT_H
#define   OPENGLPOINTLIGHT_H OpenGLPointLight

#include <OpenGLTranslationLight>

class OpenGLPointLightPrivate;
class OpenGLPointLight : public OpenGLTranslationLight
{
public:
  OpenGLPointLight();

  // Static Scalar
  static float CalculateScalar(int segments, int rings);

  // Point Light settings
  float radius() const;
  void setRadius(float r);

private:
  float m_radius;
};

inline float OpenGLPointLight::radius() const
{
  return m_radius;
}

#endif // OPENGLPOINTLIGHT_H
