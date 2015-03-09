#include "openglinstancegroup.h"

#include <KMath>
#include <KTransform3D>

#include <OpenGLRenderBlock>
#include <OpenGLDynamicBuffer>
#include <OpenGLInstance>
#include <OpenGLInstanceData>
#include <OpenGLMaterial>
#include <OpenGLMesh>

/*******************************************************************************
 * OpenGLInstanceGroupPrivate
 ******************************************************************************/
class OpenGLInstanceGroupPrivate
{
public:
  typedef OpenGLInstanceData DataType;
  typedef OpenGLInstanceData* DataPointer;
  typedef OpenGLDynamicBuffer<OpenGLInstanceData> BufferType;
  typedef std::vector<OpenGLInstance*> TransformList;

  void create();
  void setMesh(const OpenGLMesh &mesh);
  void update(const OpenGLRenderBlock &current, const OpenGLRenderBlock &previous);

  OpenGLMesh m_mesh;
  BufferType m_buffer;
  TransformList m_instances;
};

void OpenGLInstanceGroupPrivate::create()
{
  m_buffer.setUsagePattern(OpenGLBuffer::DynamicDraw);
  m_buffer.create();
}

void OpenGLInstanceGroupPrivate::setMesh(const OpenGLMesh &mesh)
{
  m_mesh = mesh;
  m_mesh.bind();
  m_buffer.bind();
  m_mesh.vertexAttribPointerDivisor(2, 3, OpenGLElementType::Float, false, OpenGLInstance::Stride(), OpenGLInstance::DiffuseOffset(), 1);
  m_mesh.vertexAttribPointerDivisor(3, 4, OpenGLElementType::Float, false, OpenGLInstance::Stride(), OpenGLInstance::SpecularOffset(), 1);
  m_mesh.vertexAttribPointerDivisor(4, 4, 4, OpenGLElementType::Float, false, OpenGLInstance::Stride(), OpenGLInstance::CurrentTransformOffset(), 1);
  m_mesh.vertexAttribPointerDivisor(8, 4, 4, OpenGLElementType::Float, false, OpenGLInstance::Stride(), OpenGLInstance::PreviousTransformOffset(), 1);
  m_mesh.vertexAttribPointerDivisor(12, 4, 4, OpenGLElementType::Float, false, OpenGLInstance::Stride(), OpenGLInstance::NormalsTransformOffset(), 1);
  m_mesh.release();
}

void OpenGLInstanceGroupPrivate::update(const OpenGLRenderBlock &current, const OpenGLRenderBlock &previous)
{
  // Map Dynamic Data
  m_buffer.bind();
  m_buffer.reserve(m_instances.size());
  BufferType::RangeAccessFlags flags =
      BufferType::RangeInvalidate
    | BufferType::RangeUnsynchronized
    | BufferType::RangeWrite;
  DataPointer dest = m_buffer.mapRange(0, m_instances.size(), flags);

  if (dest == NULL)
  {
    qFatal("Failed to map the buffer range!");
  }

  // Upload data to GPU
  DataPointer instanceDest;
  OpenGLInstance *instanceSource;
  for (int i = 0; i < m_instances.size(); ++i)
  {
    instanceDest   = &dest[i];
    instanceSource = m_instances[i];
    instanceDest->m_currModelView = current.worldToView() * Karma::ToGlm(instanceSource->currentTransform().toMatrix());
    instanceDest->m_diffuse = Karma::ToGlm(instanceSource->material().diffuse());
    instanceDest->m_normalTransform = glm::transpose(glm::inverse(instanceDest->m_currModelView));
    instanceDest->m_prevModelView = previous.worldToView() * Karma::ToGlm(instanceSource->previousTransform().toMatrix());
    instanceDest->m_specular = Karma::ToGlm(instanceSource->material().specularColor(), instanceSource->material().specularExponent());
    instanceSource->update();
  }

  // Finalize Mapping
  m_buffer.unmap();
  m_buffer.release();
}

/*******************************************************************************
 * OpenGLInstanceGroup
 ******************************************************************************/
OpenGLInstanceGroup::OpenGLInstanceGroup() :
  m_private(new OpenGLInstanceGroupPrivate)
{
  // Intentionally Empty
}

OpenGLInstanceGroup::~OpenGLInstanceGroup()
{
  delete m_private;
}

void OpenGLInstanceGroup::create()
{
  P(OpenGLInstanceGroupPrivate);
  p.create();
}

void OpenGLInstanceGroup::setMesh(const OpenGLMesh &mesh)
{
  P(OpenGLInstanceGroupPrivate);
  p.setMesh(mesh);
}

void OpenGLInstanceGroup::update(const OpenGLRenderBlock &current, const OpenGLRenderBlock &previous)
{
  P(OpenGLInstanceGroupPrivate);
  p.update(current, previous);
}

void OpenGLInstanceGroup::draw()
{
  P(OpenGLInstanceGroupPrivate);
  p.m_mesh.bind();
  p.m_mesh.drawInstanced(0, p.m_instances.size());
  p.m_mesh.release();
}

OpenGLInstance *OpenGLInstanceGroup::createInstance()
{
  P(OpenGLInstanceGroupPrivate);
  OpenGLInstance *instance = new OpenGLInstance;
  p.m_instances.push_back(instance);
  return instance;
}

OpenGLInstanceGroup::InstanceList &OpenGLInstanceGroup::instances()
{
  P(OpenGLInstanceGroupPrivate);
  return p.m_instances;
}

const OpenGLInstanceGroup::InstanceList &OpenGLInstanceGroup::instances() const
{
  P(OpenGLInstanceGroupPrivate);
  return p.m_instances;
}
