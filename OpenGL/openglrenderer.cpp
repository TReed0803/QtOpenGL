#include "openglrenderer.h"

#include <vector>
#include <KMacros>
#include <KSize>
#include <KString>
#include <OpenGLMarkerScoped>
#include <OpenGLRenderPass>
#include <OpenGLScene>
#include <OpenGLViewport>

static OpenGLRenderer *sg_renderer = 0;

class OpenGLRendererPrivate
{
public:
  typedef std::vector<OpenGLRenderPass*> RenderPassContainer;
  typedef std::vector<OpenGLViewport*> ViewportContainer;

  OpenGLRendererPrivate();

  bool m_paused;
  KSize m_screenDimensions;
  RenderPassContainer m_passes;
  ViewportContainer m_viewports;
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

void OpenGLRenderer::bind()
{
  sg_renderer = this;
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
  p.m_screenDimensions.setWidth(width);
  p.m_screenDimensions.setHeight(height);
  for (OpenGLViewport *view : p.m_viewports)
  {
    view->resize(width, height);
  }
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->resize(width, height);
  }
}

void OpenGLRenderer::commit(OpenGLViewport &view)
{
  P(OpenGLRendererPrivate);
  for (OpenGLViewport *view : p.m_viewports)
  {
    view->commit();
  }
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->commit(view);
  }
}

void OpenGLRenderer::render(OpenGLScene &scene)
{
  P(OpenGLRendererPrivate);
  unsigned int currViewport = 1;
  OpenGLMarkerScoped _("Total Render Time");
  for (OpenGLViewport *view : p.m_viewports)
  {
    OpenGLMarkerScoped _(KString("Viewport %1").arg(currViewport));
    view->bind();
    {
      OpenGLMarkerScoped _("Prepare Scene");
      commit(*view);
      scene.commit(*view);
    }
    {
      OpenGLMarkerScoped _("Render Scene");
      for (OpenGLRenderPass *pass : p.m_passes)
      {
        pass->render(scene);
      }
    }
    view->release();
    ++currViewport;
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

void OpenGLRenderer::release()
{
  sg_renderer = 0;
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

void OpenGLRenderer::registerViewport(OpenGLViewport *view)
{
  P(OpenGLRendererPrivate);
  view->resize(p.m_screenDimensions.width(), p.m_screenDimensions.height());
  p.m_viewports.push_back(view);
}

void OpenGLRenderer::activateViewport(OpenGLViewport *view)
{
  sg_renderer->registerViewport(view);
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

