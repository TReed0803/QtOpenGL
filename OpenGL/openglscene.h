#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H OpenGLScene

class OpenGLUpdateEvent;
class OpenGLInstance;
class OpenGLPointLight;
class OpenGLSphereLight;
class OpenGLRectangleLight;
class OpenGLSpotLight;
class OpenGLDirectionLight;
class OpenGLPointLightGroup;
class OpenGLSphereLightGroup;
class OpenGLSpotLightGroup;
class OpenGLDirectionLightGroup;
class OpenGLRectangleLightGroup;
class OpenGLViewport;
class OpenGLEnvironment;
#include <KUniquePointer>

class OpenGLScenePrivate;
class OpenGLScene
{
public:

  // Constructors / Destructor
  OpenGLScene();
  virtual ~OpenGLScene();

  // Virtual Scene functions
  virtual void start();
  virtual void update(OpenGLUpdateEvent *event);
  virtual void end();

  // Object Creation
  OpenGLInstance *createInstance();
  OpenGLPointLight *createPointLight();
  OpenGLSpotLight *createSpotLight();
  OpenGLSphereLight *createSphereLight();
  OpenGLRectangleLight *createRectangleLight();
  OpenGLDirectionLight *createDirectionLight();
  OpenGLPointLightGroup &pointLights();
  OpenGLSpotLightGroup &spotLights();
  OpenGLDirectionLightGroup &directionLights();
  OpenGLSphereLightGroup &sphereLights();
  OpenGLRectangleLightGroup &rectangleLights();
  void renderGeometry();
  void renderAllGeometry();
  void renderLights();
  void renderShadowedLights();
  void commit(const OpenGLViewport &view);

  // Scene stats
  OpenGLEnvironment *environment();

private:
  KUniquePointer<OpenGLScenePrivate> m_private;
};

#endif // OPENGLSCENE_H
