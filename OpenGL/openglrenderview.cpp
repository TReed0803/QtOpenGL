#include "openglrenderview.h"

#include <vector>
#include <KMacros>
#include <KSize>
#include <OpenGLRenderPass>
#include <OpenGLScene>
#include <OpenGLViewport>
#include <OpenGLMarkerScoped>
#include <OpenGLRenderPassQueue>

class OpenGLRenderViewPrivate
{
public:
  KSize m_viewDimensions;
  OpenGLViewport m_view;
  OpenGLRenderPassQueue m_passQueue;
};

OpenGLRenderView::OpenGLRenderView() :
  m_private(0)
{
  // Intentionally Empty
}

OpenGLRenderView::~OpenGLRenderView()
{
  // Intentionally Empty
}

void OpenGLRenderView::create(const OpenGLRenderPassQueue &queue, OpenGLViewport &view)
{
  m_private = new OpenGLRenderViewPrivate;
  P(OpenGLRenderViewPrivate);
  p.m_view = view;
  p.m_passQueue.create(queue);
}

void OpenGLRenderView::bind()
{
  P(OpenGLRenderViewPrivate);
  p.m_view.bind();
}

void OpenGLRenderView::resize(const KSize &size)
{
  resize(size.width(), size.height());
}

void OpenGLRenderView::resize(int width, int height)
{
  P(OpenGLRenderViewPrivate);
  p.m_view.resize(width, height);
  p.m_passQueue.resize(p.m_view.size());
}

void OpenGLRenderView::commit(OpenGLScene &scene)
{
  P(OpenGLRenderViewPrivate);
  OpenGLMarkerScoped _("Prepare Scene");
  p.m_view.commit();
  p.m_passQueue.commit(p.m_view);
  scene.commit(p.m_view);
}

void OpenGLRenderView::render(OpenGLScene &scene)
{
  P(OpenGLRenderViewPrivate);
  OpenGLMarkerScoped _("Render Scene");
  p.m_passQueue.render(scene);
}

void OpenGLRenderView::teardown()
{
  P(OpenGLRenderViewPrivate);
  p.m_passQueue.teardown();
}

void OpenGLRenderView::release()
{
  P(OpenGLRenderViewPrivate);
  p.m_view.release();
}

OpenGLRenderPassQueue *OpenGLRenderView::passes()
{
  P(OpenGLRenderViewPrivate);
  return &p.m_passQueue;
}

