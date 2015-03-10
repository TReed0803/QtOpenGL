#include "kscene.h"

#include <KMacros>
#include <OpenGLInstanceManager>
#include <OpenGLLightManager>

class KScenePrivate
{
public:
  OpenGLInstanceManager m_instanceManager;
  OpenGLLightManager m_lightManager;
};

void KScene::start()
{
  // Intentionally Empty
}

void KScene::update(KUpdateEvent *event)
{
  (void)event;
}

void KScene::end()
{
  // Intentionally Empty
}

OpenGLInstance *KScene::createInstance()
{
  P(KScenePrivate);
  return p.m_instanceManager.createInstance();
}

OpenGLPointLight *KScene::createPointLight()
{
  P(KScenePrivate);
  return p.m_lightManager.createPointLight();
}

OpenGLSpotLight *KScene::createSpotLight()
{
  P(KScenePrivate);
  return p.m_lightManager.createSpotLight();
}

OpenGLDirectionLight *KScene::createDirectionLight()
{
  P(KScenePrivate);
  return p.m_lightManager.createDirectionLight();
}

OpenGLPointLightGroup &KScene::pointLights()
{
  P(KScenePrivate);
  return p.m_lightManager.pointLights();
}

OpenGLSpotLightGroup &KScene::spotLights()
{
  P(KScenePrivate);
  return p.m_lightManager.spotLights();
}

OpenGLDirectionLightGroup &KScene::directionLights()
{
  P(KScenePrivate);
  return p.m_lightManager.directionLights();
}

void KScene::renderGeometry() const
{
  P(KScenePrivate);
  p.m_instanceManager.render();
}

void KScene::renderLights() const
{
  P(KScenePrivate);
  p.m_lightManager.render();
}

void KScene::commit(const OpenGLViewport &view)
{
  P(KScenePrivate);
  p.m_instanceManager.commit(view);
  p.m_lightManager.commit(view);
}
