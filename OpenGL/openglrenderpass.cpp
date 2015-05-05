#include "openglrenderpass.h"

OpenGLRenderPass::OpenGLRenderPass() :
  m_active(true)
{
  // Intentionally Empty
}

void OpenGLRenderPass::setActive(bool a)
{
  m_active = a;
}

bool OpenGLRenderPass::active() const
{
  return m_active;
}
