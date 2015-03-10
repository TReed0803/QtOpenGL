#include "openglrenderer.h"

#include <vector>
#include <KMacros>
#include <OpenGLRenderPass>
#include <OpenGLInstanceManager>
#include <OpenGLLightManager>

class OpenGLRendererPrivate
{
public:
  typedef std::vector<OpenGLRenderPass*> RenderPassContainer;

  OpenGLRendererPrivate();

  bool m_paused;
  RenderPassContainer m_passes;
  OpenGLInstanceManager m_instanceManager;
  OpenGLLightManager m_lightManager;
};

OpenGLRendererPrivate::OpenGLRendererPrivate() :
  m_paused(false)
{
  // Intentionally Empty
}

OpenGLRenderer::OpenGLRenderer() :
  m_private(0)
{
  // Intentionally Empty
}

void OpenGLRenderer::create()
{
  m_private = new OpenGLRendererPrivate;
  P(OpenGLRendererPrivate);
  p.m_lightManager.create();
  p.m_instanceManager.create();
}

void OpenGLRenderer::initialize()
{
  P(OpenGLRendererPrivate);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->initialize();
  }
}

void OpenGLRenderer::resize(int width, int height)
{
  P(OpenGLRendererPrivate);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->resize(width, height);
  }
}

void OpenGLRenderer::update(OpenGLRenderBlock &current, OpenGLRenderBlock &previous)
{
  P(OpenGLRendererPrivate);
  p.m_lightManager.update(current, previous);
  p.m_instanceManager.update(current, previous);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->commit(current, previous);
  }
}

void OpenGLRenderer::render()
{
  P(OpenGLRendererPrivate);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->render(*this);
  }
}

void OpenGLRenderer::teardown()
{
  P(OpenGLRendererPrivate);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->teardown();
  }
  delete m_private;
}

OpenGLInstance *OpenGLRenderer::createInstance()
{
  P(OpenGLRendererPrivate);
  return p.m_instanceManager.createInstance();
}

OpenGLPointLight *OpenGLRenderer::createPointLight()
{
  P(OpenGLRendererPrivate);
  return p.m_lightManager.createPointLight();
}

OpenGLSpotLight *OpenGLRenderer::createSpotLight()
{
  P(OpenGLRendererPrivate);
  return p.m_lightManager.createSpotLight();
}

OpenGLDirectionLight *OpenGLRenderer::createDirectionLight()
{
  P(OpenGLRendererPrivate);
  return p.m_lightManager.createDirectionLight();
}

OpenGLPointLightGroup &OpenGLRenderer::pointLights()
{
  P(OpenGLRendererPrivate);
  return p.m_lightManager.pointLights();
}

OpenGLSpotLightGroup &OpenGLRenderer::spotLights()
{
  P(OpenGLRendererPrivate);
  return p.m_lightManager.spotLights();
}

OpenGLDirectionLightGroup &OpenGLRenderer::directionLights()
{
  P(OpenGLRendererPrivate);
  return p.m_lightManager.directionLights();
}

void OpenGLRenderer::renderGeometry()
{
  P(OpenGLRendererPrivate);
  p.m_instanceManager.render();
}

void OpenGLRenderer::renderLights()
{
  P(OpenGLRendererPrivate);
  p.m_lightManager.render();
}

void OpenGLRenderer::pause(bool _p)
{
  P(OpenGLRendererPrivate);
  p.m_paused = _p;
}

bool OpenGLRenderer::isPaused() const
{
  P(OpenGLRendererPrivate);
  return p.m_paused;
}

OpenGLRenderPass *OpenGLRenderer::getPass(unsigned id)
{
  P(OpenGLRendererPrivate);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    if (pass->id() == id)
    {
      return pass;
    }
  }
  return nullptr;
}

void OpenGLRenderer::addPass(OpenGLRenderPass *pass)
{
  P(OpenGLRendererPrivate);
  p.m_passes.emplace_back(pass);
}

