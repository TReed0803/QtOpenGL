#ifndef OPENGLSPHERELIGHT_H
#define OPENGLSPHERELIGHT_H OpenGLSphereLight

#include <OpenGLAreaLight>

class OpenGLSphereLight : public OpenGLAreaLight
{
public:
  OpenGLSphereLight();

  void setRadius(float r);
  float radius() const;

private:
  float m_radius;
};

#endif // OPENGLSPHERELIGHT_H
