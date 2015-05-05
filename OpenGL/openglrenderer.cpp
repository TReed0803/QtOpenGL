#include "openglrenderer.h"

#include <vector>
#include <KMacros>
#include <KSize>
#include <KString>
#include <OpenGLMarkerScoped>
#include <OpenGLRenderPass>
#include <OpenGLScene>
#include <OpenGLViewport>
#include <OpenGLRenderPassQueue>
#include <OpenGLRenderView>
#include <OpenGLFunctions>

static OpenGLRenderer *sg_renderer = 0;

class OpenGLRendererPrivate
{
public:
  typedef std::vector<OpenGLRenderView> OpenGLRenderViewList;
  OpenGLRendererPrivate();

  bool m_paused;
  KSize m_screenDimensions;
  OpenGLRenderViewList m_renderViews;
  OpenGLRenderPassQueue m_masterQueue;
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
  P(OpenGLRendererPrivate);
  p.m_masterQueue.create();
}

void OpenGLRenderer::resize(int width, int height)
{
  P(OpenGLRendererPrivate);
  p.m_screenDimensions.setWidth(width);
  p.m_screenDimensions.setHeight(height);
  for (OpenGLRenderView &renderView: p.m_renderViews)
  {
    renderView.resize(width, height);
  }
}

void OpenGLRenderer::render(OpenGLScene &scene)
{
  P(OpenGLRendererPrivate);
  unsigned int currViewport = 1;
  OpenGLMarkerScoped _("Total Render Time");
  for (OpenGLRenderView &renderView: p.m_renderViews)
  {
    OpenGLMarkerScoped _(KString("Viewport %1").arg(currViewport));
    renderView.bind();
    renderView.commit(scene);
    renderView.render(scene);
    renderView.release();
    ++currViewport;
  }
  GL::glViewport(0, 0, p.m_screenDimensions.width(), p.m_screenDimensions.height());
}

void OpenGLRenderer::teardown()
{
  P(OpenGLRendererPrivate);
  for (OpenGLRenderView &renderView: p.m_renderViews)
  {
    renderView.teardown();
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

OpenGLRenderPass *OpenGLRenderer::pass(int id)
{
  // For now, just return the first view's pass
  P(OpenGLRendererPrivate);
  for (OpenGLRenderView &view : p.m_renderViews)
  {
    return view.passes()->pass(id);
  }
  return 0;
}

void OpenGLRenderer::registerViewport(OpenGLViewport *view)
{
  P(OpenGLRendererPrivate);
  OpenGLRenderView renderView;
  renderView.create(p.m_masterQueue, *view);
  renderView.resize(p.m_screenDimensions);
  p.m_renderViews.push_back(renderView);
}

void OpenGLRenderer::activateViewport(OpenGLViewport *view)
{
  sg_renderer->registerViewport(view);
}

void OpenGLRenderer::addPass(OpenGLRenderPass *pass)
{
  P(OpenGLRendererPrivate);
  p.m_masterQueue.addPass(pass);
}

