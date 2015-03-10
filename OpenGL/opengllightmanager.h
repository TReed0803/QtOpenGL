#ifndef OPENGLLIGHTMANAGER_H
#define OPENGLLIGHTMANAGER_H OpenGLLightManager

class OpenGLPointLight;
class OpenGLSpotLight;
class OpenGLDirectionLight;
class OpenGLPointLightGroup;
class OpenGLSpotLightGroup;
class OpenGLDirectionLightGroup;
class OpenGLRenderBlock;
#include <KUniquePointer>

class OpenGLLightManagerPrivate;
class OpenGLLightManager
{
public:
  OpenGLLightManager();
  ~OpenGLLightManager();
  void create();
  void update(OpenGLRenderBlock &current, OpenGLRenderBlock &previous);
  void render();
  OpenGLPointLight *createPointLight();
  OpenGLSpotLight *createSpotLight();
  OpenGLDirectionLight *createDirectionLight();
  OpenGLPointLightGroup &pointLights();
  OpenGLSpotLightGroup &spotLights();
  OpenGLDirectionLightGroup &directionLights();
private:
  KUniquePointer<OpenGLLightManagerPrivate> m_private;
};

#endif // OPENGLLIGHTMANAGER_H
