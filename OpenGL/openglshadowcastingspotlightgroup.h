#ifndef OPENGLSHADOWCASTINGSPOTLIGHTGROUP_H
#define OPENGLSHADOWCASTINGSPOTLIGHTGROUP_H

#include <vector>
#include <OpenGLSpotLight>

class OpenGLShadowCastingSpotLightGroup
{
public:
  OpenGLShadowCastingSpotLightGroup();
  ~OpenGLShadowCastingSpotLightGroup();

  OpenGLSpotLight *createLight();
  void update();
  void draw();

private:
  std::vector<OpenGLSpotLight *> m_lights;
};

#endif // OPENGLSHADOWCASTINGSPOTLIGHTGROUP_H
