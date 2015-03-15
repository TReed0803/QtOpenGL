#include "openglviewport.h"

#include <KMacros>
#include <KVector2D>
#include <KCamera3D>
#include <KSize>
#include <KSizeF>
#include <OpenGLRenderBlock>
#include <OpenGLRenderer>

class OpenGLViewportPrivate
{
public:
  OpenGLViewportPrivate();
  ~OpenGLViewportPrivate();

  OpenGLRenderBlock &currentRenderBlock();
  OpenGLRenderBlock &previousRenderBlock();
  const OpenGLRenderBlock &currentRenderBlock() const;
  const OpenGLRenderBlock &previousRenderBlock() const;
  void swapRenderBlocks();
  void fixRenderBlocks();
  void updateRenderBlocks();
  bool viewportDirty();

  float m_aspectRatio;
  float m_nearPlane;
  float m_farPlane;
  bool m_dirty;
  KVector2D m_origin;
  KSizeF m_dimensions;
  KSize m_pixelDimensions;
  KSize m_screenSize;
  const KCamera3D *m_camera;
  OpenGLRenderBlock m_renderBlocks[2];
  int m_renderBlockIndex[2];
  int m_renderBlockBindings[2];
};

OpenGLViewportPrivate::OpenGLViewportPrivate() :
  m_nearPlane(0.1f), m_farPlane(1000.0f), m_origin(0.0f, 0.0f), m_dimensions(1.0f, 1.0f)
{
  m_renderBlockIndex[0] = 0;    // Current Index
  m_renderBlockIndex[1] = 1;    // Previous Index
  m_renderBlockBindings[0] = 0; // Current Binding
  m_renderBlockBindings[1] = 0; // Previous Binding
}

OpenGLViewportPrivate::~OpenGLViewportPrivate()
{
  // Intentionally Empty
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

bool OpenGLViewportPrivate::viewportDirty()
{
  // Note: Viewport is dirty if the camera is NOT dirty,
  //       but the values of the camera transform do not match.
  return true;
}

OpenGLViewport::OpenGLViewport() :
  m_private(0)
{
  // Intentionally Empty
}

OpenGLViewport::~OpenGLViewport()
{
  // Intentionally Empty
}

void OpenGLViewport::create()
{
  m_private = new OpenGLViewportPrivate;
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
  p.m_screenSize.setWidth(width);
  p.m_screenSize.setHeight(height);
  p.m_pixelDimensions.setWidth(width * p.m_dimensions.width());
  p.m_pixelDimensions.setHeight(height * p.m_dimensions.height());
  p.m_aspectRatio = p.m_pixelDimensions.width() / float(p.m_pixelDimensions.height());
  p.m_dirty = true;
}

void OpenGLViewport::resize(const KSize &size)
{
  resize(size.width(), size.height());
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

void OpenGLViewport::setRegion(float x, float y, float w, float h)
{
  P(OpenGLViewportPrivate);
  p.m_origin.setX(x);
  p.m_origin.setY(y);
  p.m_dimensions.setWidth(w);
  p.m_dimensions.setHeight(h);
  p.m_dirty = true;
}

const KSize &OpenGLViewport::size() const
{
  P(const OpenGLViewportPrivate);
  return p.m_pixelDimensions;
}

const KSize &OpenGLViewport::screenSize() const
{
  P(const OpenGLViewportPrivate);
  return p.m_screenSize;
}

const KVector2D &OpenGLViewport::origin() const
{
  P(const OpenGLViewportPrivate);
  return p.m_origin;
}

const KSizeF &OpenGLViewport::region() const
{
  P(const OpenGLViewportPrivate);
  return p.m_dimensions;
}

void OpenGLViewport::commit()
{
  P(OpenGLViewportPrivate);
  if (!p.m_camera) return;

  // Update GPU Data for camera view
  if (p.m_camera->dirty() || p.viewportDirty())
  {
    p.swapRenderBlocks();
    KVector2D offset(0.0f, p.m_pixelDimensions.height());
    p.currentRenderBlock().setNearFar(p.m_nearPlane, p.m_farPlane);
    p.currentRenderBlock().setOrigin(offset);
    p.currentRenderBlock().setDimensions(p.m_pixelDimensions);
    p.currentRenderBlock().setPerspective(p.m_camera->fieldOfView(), p.m_aspectRatio, p.m_nearPlane, p.m_farPlane);
    p.currentRenderBlock().setViewMatrix(p.m_camera->toMatrix());
  }
  else
  {
    p.fixRenderBlocks();
  }
  p.updateRenderBlocks();
}

void OpenGLViewport::activate()
{
  OpenGLRenderer::activateViewport(this);
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

