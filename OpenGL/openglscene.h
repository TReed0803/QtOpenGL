#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H OpenGLScene

class KUpdateEvent;
class OpenGLInstance;
class OpenGLPointLight;
class OpenGLSpotLight;
class OpenGLDirectionLight;
class OpenGLPointLightGroup;
class OpenGLSpotLightGroup;
class OpenGLDirectionLightGroup;
class OpenGLViewport;
#include <KUniquePointer>

class OpenGLScenePrivate;
class OpenGLScene
{
public:

  // Constructors / Destructor
  OpenGLScene();
  ~OpenGLScene();

  // Virtual Scene functions
  virtual void start();
  virtual void update(KUpdateEvent *event);
  virtual void end();

  // Object Creation
  OpenGLInstance *createInstance();
  OpenGLPointLight *createPointLight();
  OpenGLSpotLight *createSpotLight();
  OpenGLDirectionLight *createDirectionLight();
  OpenGLPointLightGroup &pointLights();
  OpenGLSpotLightGroup &spotLights();
  OpenGLDirectionLightGroup &directionLights();
  void renderGeometry();
  void renderLights();
  void renderShadowedLights();
  void commit(const OpenGLViewport &view);

private:
  KUniquePointer<OpenGLScenePrivate> m_private;
};

#endif // OPENGLSCENE_H
