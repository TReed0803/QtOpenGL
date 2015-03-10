#include "openglviewport.h"

#include <KMacros>
#include <KCamera3D>
#include <KSize>
#include <OpenGLRenderBlock>

class OpenGLViewportPrivate
{
public:
  OpenGLViewportPrivate();

  OpenGLRenderBlock &currentRenderBlock();
  OpenGLRenderBlock &previousRenderBlock();
  const OpenGLRenderBlock &currentRenderBlock() const;
  const OpenGLRenderBlock &previousRenderBlock() const;
  void swapRenderBlocks();
  void fixRenderBlocks();
  void updateRenderBlocks();

  float m_aspectRatio;
  float m_nearPlane;
  float m_farPlane;
  bool m_dirty;
  KSize m_dimensions;
  const KCamera3D *m_camera;
  OpenGLRenderBlock m_renderBlocks[2];
  int m_renderBlockIndex[2];
  int m_renderBlockBindings[2];
};

OpenGLViewportPrivate::OpenGLViewportPrivate()
{
  m_renderBlockIndex[0] = 0;    // Current Index
  m_renderBlockIndex[1] = 1;    // Previous Index
  m_renderBlockBindings[0] = 0; // Current Binding
  m_renderBlockBindings[1] = 0; // Previous Binding
}

OpenGLRenderBlock &OpenGLViewportPrivate::currentRenderBlock()
{
  return m_renderBlocks[m_renderBlockBindings[0]];
}

OpenGLRenderBlock &OpenGLViewportPrivate::previousRenderBlock()
{
  return m_renderBlocks[m_renderBlockBindings[1]];
}

const OpenGLRenderBlock &OpenGLViewportPrivate::currentRenderBlock() const
{
  return m_renderBlocks[m_renderBlockBindings[0]];
}

const OpenGLRenderBlock &OpenGLViewportPrivate::previousRenderBlock() const
{
  return m_renderBlocks[m_renderBlockBindings[1]];
}

void OpenGLViewportPrivate::swapRenderBlocks()
{
  // Update the binding indices of each render block
  std::swap(m_renderBlockIndex[0], m_renderBlockIndex[1]);
  m_renderBlockBindings[0] = m_renderBlockIndex[0];
  m_renderBlockBindings[1] = m_renderBlockIndex[1];
}

void OpenGLViewportPrivate::fixRenderBlocks()
{
  // Current = Previous (No camera motion applied)
  m_renderBlockBindings[1] = m_renderBlockBindings[0];
}

void OpenGLViewportPrivate::updateRenderBlocks()
{
  // Update previous/current render block data (if needed)
  for (int i = 0; i < 2; ++i)
  {
    if (m_renderBlocks[i].dirty())
    {
      m_renderBlocks[i].bind();
      m_renderBlocks[i].update();
      m_renderBlocks[i].release();
    }
  }
}

OpenGLViewport::OpenGLViewport() :
  m_private(new OpenGLViewportPrivate)
{
  // Intentionally Empty
}

OpenGLViewport::~OpenGLViewport()
{
  // Intentionally Empty
}

void OpenGLViewport::create()
{
  P(OpenGLViewportPrivate);
  for (int i = 0; i < 2; ++i)
  {
    p.m_renderBlocks[i].create();
    p.m_renderBlocks[i].setUsagePattern(OpenGLBuffer::DynamicDraw);
    p.m_renderBlocks[i].bind();
    p.m_renderBlocks[i].allocate();
    p.m_renderBlocks[i].release();
  }
}

void OpenGLViewport::bind()
{
  P(OpenGLViewportPrivate);
  p.currentRenderBlock().bindBase(1);
  p.previousRenderBlock().bindBase(2);
}

void OpenGLViewport::release()
{
  OpenGLUniformBufferObject::bindBufferId(1, 0);
  OpenGLUniformBufferObject::bindBufferId(2, 0);
}

void OpenGLViewport::resize(int width, int height)
{
  P(OpenGLViewportPrivate);
  p.m_dimensions.setWidth(width);
  p.m_dimensions.setHeight(height);
  p.m_aspectRatio = width / float(height);
  p.m_dirty = true;
}

void OpenGLViewport::setCamera(const KCamera3D &camera)
{
  P(OpenGLViewportPrivate);
  p.m_camera = &camera;
  p.m_dirty = true;
}

void OpenGLViewport::setNearFar(float nearPlane, float farPlane)
{
  P(OpenGLViewportPrivate);
  p.m_nearPlane = nearPlane;
  p.m_farPlane = farPlane;
  p.m_dirty = true;
}

void OpenGLViewport::setWidthHeight(int width, int height)
{
  P(OpenGLViewportPrivate);
  p.m_dimensions.setWidth(width);
  p.m_dimensions.setHeight(height);
  p.m_dirty = true;
}

void OpenGLViewport::update()
{
  P(OpenGLViewportPrivate);
  if (!p.m_camera) return;

  // Update GPU Data for camera view
  if (p.m_camera->dirty())
  {
    p.swapRenderBlocks();
    p.currentRenderBlock().setNearFar(p.m_nearPlane, p.m_farPlane);
    p.currentRenderBlock().setDimensions(p.m_dimensions);
    p.currentRenderBlock().setPerspective(p.m_camera->fieldOfView(), p.m_aspectRatio, p.m_nearPlane, p.m_farPlane);
    p.currentRenderBlock().setViewMatrix(p.m_camera->toMatrix());
  }
  else
  {
    p.fixRenderBlocks();
  }
  p.updateRenderBlocks();
}

const OpenGLRenderBlock &OpenGLViewport::previous() const
{
  P(const OpenGLViewportPrivate);
  return p.currentRenderBlock();
}

const OpenGLRenderBlock &OpenGLViewport::current() const
{
  P(const OpenGLViewportPrivate);
  return p.previousRenderBlock();
}

