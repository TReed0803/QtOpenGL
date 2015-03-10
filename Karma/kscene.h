#ifndef KSCENE_H
#define KSCENE_H KScene

class KUpdateEvent;
class OpenGLInstance;
class OpenGLPointLight;
class OpenGLSpotLight;
class OpenGLDirectionLight;
class OpenGLPointLightGroup;
class OpenGLSpotLightGroup;
class OpenGLDirectionLightGroup;
class OpenGLViewport;
#include <KAbstractScene>

class KScenePrivate;
class KScene : public KAbstractScene
{
public:
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
  void renderGeometry() const;
  void renderLights() const;
  void commit(const OpenGLViewport &view);

private:
  KScenePrivate *m_private;
};

#endif // KSCENE_H
