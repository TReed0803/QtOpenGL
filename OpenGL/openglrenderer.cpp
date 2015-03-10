#include "openglrenderer.h"

#include <vector>
#include <KMacros>
#include <OpenGLRenderPass>

class OpenGLRendererPrivate
{
public:
  typedef std::vector<OpenGLRenderPass*> RenderPassContainer;

  OpenGLRendererPrivate();

  bool m_paused;
  RenderPassContainer m_passes;
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

void OpenGLRenderer::commit(const OpenGLViewport &view)
{
  P(OpenGLRendererPrivate);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->commit(view);
  }
}

void OpenGLRenderer::render(const KScene &scene)
{
  P(OpenGLRendererPrivate);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->render(scene);
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

