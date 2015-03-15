#include "opengllightmanager.h"

#include <KMacros>
#include <OpenGLPointLightGroup>
#include <OpenGLSpotLightGroup>
#include <OpenGLRenderBlock>
#include <OpenGLDirectionLightGroup>

class OpenGLLightManagerPrivate
{
public:
  OpenGLSpotLightGroup m_spotLights;
  OpenGLPointLightGroup m_pointLights;
  OpenGLDirectionLightGroup m_directionLights;
};

OpenGLLightManager::OpenGLLightManager() :
  m_private(new OpenGLLightManagerPrivate)
{
  // Intentionally Empty
}

OpenGLLightManager::~OpenGLLightManager()
{
  // Intentionally Empty
}

void OpenGLLightManager::create()
{
  P(OpenGLLightManagerPrivate);
  p.m_spotLights.create();
  p.m_spotLights.setMesh(":/resources/objects/spotLight.obj");
  p.m_pointLights.create();
  p.m_pointLights.setMesh(":/resources/objects/pointLight.obj");
  p.m_directionLights.create();
  p.m_directionLights.setMesh(":/resources/objects/quad.obj");
}

void OpenGLLightManager::commit(const OpenGLViewport &view)
{
  P(OpenGLLightManagerPrivate);
  p.m_spotLights.commit(view);
  p.m_pointLights.commit(view);
  p.m_directionLights.commit(view);
}

void OpenGLLightManager::render()
{
  P(OpenGLLightManagerPrivate);
  p.m_spotLights.draw();
  p.m_pointLights.draw();
  p.m_directionLights.draw();
}

void OpenGLLightManager::renderShadowed(OpenGLScene &scene)
{
  P(OpenGLLightManagerPrivate);
  p.m_spotLights.drawShadowed(scene);
  p.m_pointLights.drawShadowed(scene);
  p.m_directionLights.drawShadowed(scene);
}

OpenGLPointLight *OpenGLLightManager::createPointLight()
{
  P(OpenGLLightManagerPrivate);
  return p.m_pointLights.createLight();
}

OpenGLSpotLight *OpenGLLightManager::createSpotLight()
{
  P(OpenGLLightManagerPrivate);
  return p.m_spotLights.createLight();
}

OpenGLDirectionLight *OpenGLLightManager::createDirectionLight()
{
  P(OpenGLLightManagerPrivate);
  return p.m_directionLights.createLight();
}

OpenGLPointLightGroup &OpenGLLightManager::pointLights()
{
  P(OpenGLLightManagerPrivate);
  return p.m_pointLights;
}

OpenGLSpotLightGroup &OpenGLLightManager::spotLights()
{
  P(OpenGLLightManagerPrivate);
  return p.m_spotLights;
}

OpenGLDirectionLightGroup &OpenGLLightManager::directionLights()
{
  P(OpenGLLightManagerPrivate);
  return p.m_directionLights;
}

