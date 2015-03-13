#include "openglrenderblock.h"

#include <KMacros>
#include <KMath>
#include <KMatrix4x4>
#include <KSize>
#include <KVector2D>
#include <OpenGLRenderBlockData>

class OpenGLRenderBlockPrivate
{
public:
  OpenGLRenderBlockPrivate();
  bool m_dirty;
  OpenGLRenderBlockData m_blockData;
  void updateCombinationMatrices();
};

OpenGLRenderBlockPrivate::OpenGLRenderBlockPrivate() :
  m_dirty(false)
{
  // Intentionally Empty
}

void OpenGLRenderBlockPrivate::updateCombinationMatrices()
{
  m_blockData.m_viewProjection = m_blockData.m_projection * m_blockData.m_view;
  m_blockData.i_viewProjection = m_blockData.i_view * m_blockData.i_projection;
}

OpenGLRenderBlock::OpenGLRenderBlock() :
  m_private(new OpenGLRenderBlockPrivate)
{
  // Intentionally Empty
}

OpenGLRenderBlock::~OpenGLRenderBlock()
{
  // Intentionally Empty
}

void OpenGLRenderBlock::setViewMatrix(const KMatrix4x4 &view)
{
  P(OpenGLRenderBlockPrivate);
  p.m_dirty = true;
  p.m_blockData.m_view = Karma::ToGlm(view);
  p.m_blockData.i_view = glm::inverse(p.m_blockData.m_view);
  p.updateCombinationMatrices();
}

void OpenGLRenderBlock::setPerspective(float fovy, float aspectRatio, float nearPlane, float farPlane)
{
  KMatrix4x4 persp;
  persp.perspective(fovy, aspectRatio, nearPlane, farPlane);
  setPerspectiveMatrix(persp);
}

void OpenGLRenderBlock::setPerspectiveMatrix(const KMatrix4x4 &perspective)
{
  P(OpenGLRenderBlockPrivate);
  p.m_dirty = true;
  p.m_blockData.m_projection = Karma::ToGlm(perspective);
  p.m_blockData.i_projection = glm::inverse(p.m_blockData.m_projection);
  p.updateCombinationMatrices();
}

void OpenGLRenderBlock::setOrigin(const KVector2D &origin)
{
  P(OpenGLRenderBlockPrivate);
  p.m_blockData.v_offset = Karma::ToGlm(origin.x(), origin.y());
}

void OpenGLRenderBlock::setNearFar(float nearPlane, float farPlane)
{
  P(OpenGLRenderBlockPrivate);
  p.m_dirty = true;
  p.m_blockData.f_nearPlane = nearPlane;
  p.m_blockData.f_farPlane = farPlane;
  p.m_blockData.f_diffPlane = farPlane - nearPlane;
  p.m_blockData.f_sumPlane = farPlane + nearPlane;
}

void OpenGLRenderBlock::setDimensions(int width, int height)
{
  P(OpenGLRenderBlockPrivate);
  p.m_dirty = true;
  p.m_blockData.v_dimensions = glm::vec2(float(width), float(height));
}

void OpenGLRenderBlock::setDimensions(const KSize &size)
{
  setDimensions(size.width(), size.height());
}

const glm::mat4 &OpenGLRenderBlock::worldToView() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.m_view;
}

const glm::mat4 &OpenGLRenderBlock::viewToWorld() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.i_view;
}

const glm::mat4 &OpenGLRenderBlock::worldToPersp() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.m_viewProjection;
}

const glm::mat4 &OpenGLRenderBlock::perspToWorld() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.i_viewProjection;
}

const glm::mat4 &OpenGLRenderBlock::viewToPersp() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.m_projection;
}

const glm::mat4 &OpenGLRenderBlock::perspToView() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.i_projection;
}

float OpenGLRenderBlock::nearPlane() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.f_nearPlane;
}

float OpenGLRenderBlock::farPlane() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.f_farPlane;
}

float OpenGLRenderBlock::diffPlane() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.f_diffPlane;
}

float OpenGLRenderBlock::sumPlane() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.f_sumPlane;
}

int OpenGLRenderBlock::width() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.v_dimensions.x;
}

int OpenGLRenderBlock::height() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_blockData.v_dimensions.y;
}

void OpenGLRenderBlock::update()
{
  P(OpenGLRenderBlockPrivate);

  if (!p.m_dirty) return;

  OpenGLBuffer::RangeAccessFlags flags =
      OpenGLBuffer::RangeInvalidate
    | OpenGLBuffer::RangeUnsynchronized
    | OpenGLBuffer::RangeWrite;
  OpenGLRenderBlockData *data = (OpenGLRenderBlockData*)mapRange(0, sizeof(OpenGLRenderBlockData), flags);
  std::memcpy(data, &p.m_blockData, sizeof(OpenGLRenderBlockData));
  unmap();

  p.m_dirty = false;
}

void OpenGLRenderBlock::allocate()
{
  OpenGLUniformBufferObject::allocate(sizeof(OpenGLRenderBlockData));
}

bool OpenGLRenderBlock::dirty() const
{
  P(const OpenGLRenderBlockPrivate);
  return p.m_dirty;
}
