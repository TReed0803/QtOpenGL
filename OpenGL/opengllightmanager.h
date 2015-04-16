#ifndef OPENGLLIGHTMANAGER_H
#define OPENGLLIGHTMANAGER_H OpenGLLightManager

class OpenGLPointLight;
class OpenGLSpotLight;
class OpenGLDirectionLight;
class OpenGLSphereLight;
class OpenGLRectangleLight;
class OpenGLPointLightGroup;
class OpenGLSpotLightGroup;
class OpenGLDirectionLightGroup;
class OpenGLSphereLightGroup;
class OpenGLRectangleLightGroup;
class OpenGLScene;
class OpenGLViewport;
#include <KUniquePointer>

class OpenGLLightManagerPrivate;
class OpenGLLightManager
{
public:
  OpenGLLightManager();
  ~OpenGLLightManager();
  void create();
  void commit(const OpenGLViewport &view);
  void render();
  void renderShadowed(OpenGLScene &scene);
  OpenGLPointLight *createPointLight();
  OpenGLSpotLight *createSpotLight();
  OpenGLDirectionLight *createDirectionLight();
  OpenGLSphereLight *createSphereLight();
  OpenGLRectangleLight *createRectangleLight();
  OpenGLPointLightGroup &pointLights();
  OpenGLSpotLightGroup &spotLights();
  OpenGLDirectionLightGroup &directionLights();
  OpenGLSphereLightGroup &sphereLights();
  OpenGLRectangleLightGroup &rectangleLights();
private:
  KUniquePointer<OpenGLLightManagerPrivate> m_private;
};

#endif // OPENGLLIGHTMANAGER_H
