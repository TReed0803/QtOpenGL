#include "openglscene.h"

#include <KMacros>
#include <OpenGLInstanceManager>
#include <OpenGLLightManager>
#include <OpenGLEnvironment>

class OpenGLScenePrivate
{
public:
  OpenGLInstanceManager m_instanceManager;
  OpenGLLightManager m_lightManager;
  OpenGLEnvironment m_environment;
};

OpenGLScene::OpenGLScene() :
  m_private(0)
{
  // Intentionally Empty
}

OpenGLScene::~OpenGLScene()
{
  // Intentionally Empty
}

void OpenGLScene::start()
{
  m_private = new OpenGLScenePrivate;
  P(OpenGLScenePrivate);
  p.m_instanceManager.create();
  p.m_lightManager.create();
  p.m_environment.create();
}

void OpenGLScene::update(OpenGLUpdateEvent *event)
{
  (void)event;
}

void OpenGLScene::end()
{
  m_private = 0;
}

OpenGLInstance *OpenGLScene::createInstance()
{
  P(OpenGLScenePrivate);
  return p.m_instanceManager.createInstance();
}

OpenGLPointLight *OpenGLScene::createPointLight()
{
  P(OpenGLScenePrivate);
  return p.m_lightManager.createPointLight();
}

OpenGLSpotLight *OpenGLScene::createSpotLight()
{
  P(OpenGLScenePrivate);
  return p.m_lightManager.createSpotLight();
}

OpenGLSphereLight *OpenGLScene::createSphereLight()
{
  P(OpenGLScenePrivate);
  return p.m_lightManager.createSphereLight();
}

OpenGLRectangleLight *OpenGLScene::createRectangleLight()
{
  P(OpenGLScenePrivate);
  return p.m_lightManager.createRectangleLight();
}

OpenGLDirectionLight *OpenGLScene::createDirectionLight()
{
  P(OpenGLScenePrivate);
  return p.m_lightManager.createDirectionLight();
}

OpenGLPointLightGroup &OpenGLScene::pointLights()
{
  P(OpenGLScenePrivate);
  return p.m_lightManager.pointLights();
}

OpenGLSpotLightGroup &OpenGLScene::spotLights()
{
  P(OpenGLScenePrivate);
  return p.m_lightManager.spotLights();
}

OpenGLDirectionLightGroup &OpenGLScene::directionLights()
{
  P(OpenGLScenePrivate);
  return p.m_lightManager.directionLights();
}

OpenGLSphereLightGroup &OpenGLScene::sphereLights()
{
  P(OpenGLScenePrivate);
  return p.m_lightManager.sphereLights();
}

OpenGLRectangleLightGroup &OpenGLScene::rectangleLights()
{
  P(OpenGLScenePrivate);
  return p.m_lightManager.rectangleLights();
}

void OpenGLScene::renderGeometry()
{
  P(OpenGLScenePrivate);
  p.m_instanceManager.render();
}

void OpenGLScene::renderAllGeometry()
{
  P(OpenGLScenePrivate);
  p.m_instanceManager.renderAll();
}

void OpenGLScene::renderLights()
{
  P(OpenGLScenePrivate);
  p.m_lightManager.render();
}

void OpenGLScene::renderShadowedLights()
{
  P(OpenGLScenePrivate);
  p.m_lightManager.renderShadowed(*this);
}

void OpenGLScene::commit(const OpenGLViewport &view)
{
  P(OpenGLScenePrivate);
  p.m_instanceManager.commit(view);
  p.m_lightManager.commit(view);
}

OpenGLEnvironment *OpenGLScene::environment()
{
  P(OpenGLScenePrivate);
  return &p.m_environment;
}
