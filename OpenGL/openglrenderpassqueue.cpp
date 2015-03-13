#include "openglrenderpassqueue.h"

#include <vector>
#include <KMacros>
#include <KSize>
#include <OpenGLRenderPass>

class OpenGLRenderPassQueuePrivate
{
public:
  typedef OpenGLRenderPassQueue::RenderPassContainer RenderPassContainer;
  RenderPassContainer m_passes;
};

OpenGLRenderPassQueue::OpenGLRenderPassQueue() :
  m_private(0)
{
  // Intentionally Empty
}

OpenGLRenderPassQueue::~OpenGLRenderPassQueue()
{
  // Intentionally Empty
}

void OpenGLRenderPassQueue::create()
{
  m_private = new OpenGLRenderPassQueuePrivate;
}

void OpenGLRenderPassQueue::create(const OpenGLRenderPassQueue &queue)
{
  m_private = new OpenGLRenderPassQueuePrivate;
  P(OpenGLRenderPassQueuePrivate);
  for (const OpenGLRenderPass *pass : queue.passes())
  {
    OpenGLRenderPass *newPass = pass->clone();
    newPass->initialize();
    p.m_passes.emplace_back(newPass);
  }
}

void OpenGLRenderPassQueue::resize(const KSize &size)
{
  resize(size.width(), size.height());
}

void OpenGLRenderPassQueue::resize(int width, int height)
{
  P(OpenGLRenderPassQueuePrivate);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->resize(width, height);
  }
}

void OpenGLRenderPassQueue::commit(OpenGLViewport &view)
{
  P(OpenGLRenderPassQueuePrivate);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->commit(view);
  }
}

void OpenGLRenderPassQueue::render(OpenGLScene &scene)
{
  P(OpenGLRenderPassQueuePrivate);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->render(scene);
  }
}

void OpenGLRenderPassQueue::teardown()
{
  P(OpenGLRenderPassQueuePrivate);
  for (OpenGLRenderPass *pass : p.m_passes)
  {
    pass->teardown();
  }
}

OpenGLRenderPassQueue::RenderPassContainer &OpenGLRenderPassQueue::passes()
{
  P(OpenGLRenderPassQueuePrivate);
  return p.m_passes;
}

void OpenGLRenderPassQueue::addPass(OpenGLRenderPass *pass)
{
  P(OpenGLRenderPassQueuePrivate);
  p.m_passes.emplace_back(pass);
}

OpenGLRenderPassQueue::ConstRenderPassContainer &OpenGLRenderPassQueue::passes() const
{
  P(const OpenGLRenderPassQueuePrivate);
  return p.m_passes;
}

