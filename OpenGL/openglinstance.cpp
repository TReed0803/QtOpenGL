#include "openglinstance.h"

#include <OpenGLMaterial>
#include <OpenGLMesh>
#include <KTransform3D>
#include <KMacros>
#include <OpenGLUniformBufferObject>
#include <OpenGLBindings>
#include <OpenGLInstanceData>
#include <OpenGLViewport>
#include <OpenGLRenderBlock>

class OpenGLInstancePrivate
{
public:
  KTransform3D m_currTransform;
  KTransform3D m_prevTransform;
  OpenGLMaterial m_material;
  OpenGLMesh m_mesh;
  OpenGLUniformBufferObject m_buffer;
};

OpenGLInstance::OpenGLInstance() :
  m_private(new OpenGLInstancePrivate)
{
  P(OpenGLInstancePrivate);
  p.m_buffer.create();
  p.m_buffer.bind();
  p.m_buffer.allocate(sizeof(OpenGLInstanceData));
  p.m_buffer.release();
}

OpenGLInstance::~OpenGLInstance()
{
  delete m_private;
}

void OpenGLInstance::bind()
{
  P(OpenGLInstancePrivate);
  p.m_buffer.bindBase(K_OBJECT_BINDING);
}

void OpenGLInstance::commit(const OpenGLViewport &viewport)
{
  P(OpenGLInstancePrivate);
  OpenGLBuffer::RangeAccessFlags flags =
    OpenGLBuffer::RangeUnsynchronized   |
    OpenGLBuffer::RangeInvalidateBuffer |
    OpenGLBuffer::RangeWrite;

  p.m_buffer.bind();

  // Send data to the GPU
  {
    OpenGLInstanceData *data = (OpenGLInstanceData*)p.m_buffer.mapRange(0, sizeof(OpenGLInstanceData), flags);
    data->m_currModelView = viewport.current().worldToView()  * Karma::ToGlm(currentTransform().toMatrix() );
    data->m_prevModelView = viewport.previous().worldToView() * Karma::ToGlm(previousTransform().toMatrix());
    data->m_normalTransform = glm::transpose(glm::inverse(data->m_currModelView));
    p.m_buffer.unmap();
  }

  update(); // Updates current/previous pairs
  p.m_buffer.release();
}

void OpenGLInstance::release()
{
  OpenGLUniformBufferObject::bindBufferId(K_OBJECT_BINDING, 0);
}

void OpenGLInstance::draw()
{

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

void OpenGLInstance::setMesh(const OpenGLMesh &mesh)
{
  P(OpenGLInstancePrivate);
  p.m_mesh = mesh;
}

const OpenGLMesh &OpenGLInstance::mesh() const
{
  P(const OpenGLInstancePrivate);
  return p.m_mesh;
}

OpenGLMesh &OpenGLInstance::mesh()
{
  P(OpenGLInstancePrivate);
  return p.m_mesh;
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

KAabbBoundingVolume OpenGLInstance::aabb() const
{
  P(const OpenGLInstancePrivate);
  return p.m_mesh.aabb() * p.m_currTransform.toMatrix();
}
