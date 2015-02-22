#include "openglinstance.h"

#include <OpenGLMaterial>
#include <KTransform3D>

class OpenGLInstancePrivate
{
public:
  KTransform3D m_currTransform;
  KTransform3D m_prevTransform;
  OpenGLMaterial m_material;
};

#define P(c) c &p = *m_private
OpenGLInstance::OpenGLInstance() :
  m_private(new OpenGLInstancePrivate)
{
  // Intentionally Empty
}

OpenGLInstance::~OpenGLInstance()
{
  delete m_private;
}

KTransform3D &OpenGLInstance::transform()
{
  P(OpenGLInstancePrivate);
  return p.m_currTransform;
}

KTransform3D &OpenGLInstance::currentTransform()
{
  P(OpenGLInstancePrivate);
  return p.m_currTransform;
}

KTransform3D &OpenGLInstance::previousTransform()
{
  P(OpenGLInstancePrivate);
  return p.m_prevTransform;
}

void OpenGLInstance::setMaterial(const OpenGLMaterial &mat)
{
  P(OpenGLInstancePrivate);
  p.m_material = mat;
}

OpenGLMaterial &OpenGLInstance::material()
{
  P(OpenGLInstancePrivate);
  return p.m_material;
}

const OpenGLMaterial &OpenGLInstance::material() const
{
  P(OpenGLInstancePrivate);
  return p.m_material;
}

void OpenGLInstance::update()
{
  P(OpenGLInstancePrivate);
  p.m_prevTransform = p.m_currTransform;
}
