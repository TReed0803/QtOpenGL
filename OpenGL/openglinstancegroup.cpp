#include "openglinstancegroup.h"
#include "openglbuffer.h"
#include "openglmesh.h"
#include "openglvertexarrayobject.h"
#include "openglfunctions.h"
#include "openglinstance.h"
#include "openglmaterial.h"

#include <vector>

#include <KColor>
#include <KTransform3D>

///
class OpenGLInstanceGroupPrivate
{
public:
  typedef std::vector<OpenGLInstance*> TransformList;

  OpenGLInstanceGroupPrivate();

  uint64_t m_bufferSize;
  OpenGLMesh m_mesh;
  OpenGLBuffer m_buffer;
  TransformList m_instances;
};

OpenGLInstanceGroupPrivate::OpenGLInstanceGroupPrivate() :
  m_bufferSize(0), m_buffer(OpenGLBuffer::VertexBuffer)
{
  m_buffer.setUsagePattern(OpenGLBuffer::DynamicDraw);
  m_buffer.create();
}

///

OpenGLInstanceGroup::OpenGLInstanceGroup(QObject *parent) :
  QObject(parent), m_private(new OpenGLInstanceGroupPrivate)
{
  // Intentionally Empty
}

void OpenGLInstanceGroup::setMesh(const OpenGLMesh &mesh)
{
  P(OpenGLInstanceGroupPrivate);
  p.m_mesh = mesh;
  p.m_mesh.bind();
  p.m_buffer.bind();
  p.m_mesh.vertexAttribPointerDivisor(2, 3, OpenGLElementType::Float, false, OpenGLInstance::Stride(), OpenGLInstance::DiffuseOffset(), 1);
  p.m_mesh.vertexAttribPointerDivisor(3, 4, OpenGLElementType::Float, false, OpenGLInstance::Stride(), OpenGLInstance::SpecularOffset(), 1);
  p.m_mesh.vertexAttribPointerDivisor(4, 4, 4, OpenGLElementType::Float, false, OpenGLInstance::Stride(), OpenGLInstance::CurrentTransformOffset(), 1);
  p.m_mesh.vertexAttribPointerDivisor(8, 4, 4, OpenGLElementType::Float, false, OpenGLInstance::Stride(), OpenGLInstance::PreviousTransformOffset(), 1);
  p.m_mesh.vertexAttribPointerDivisor(12, 4, 4, OpenGLElementType::Float, false, OpenGLInstance::Stride(), OpenGLInstance::NormalsTransformOffset(), 1);
  p.m_mesh.release();
}

void OpenGLInstanceGroup::update(KMatrix4x4 const &currWorldToCamera, KMatrix4x4 const &prevWorldToCamera)
{
  P(OpenGLInstanceGroupPrivate);
  uint64_t required = 0;
  required += p.m_instances.size() * sizeof(GLfloat) * 3;  // vec3:diffuse
  required += p.m_instances.size() * sizeof(GLfloat) * 4;  // vec3:specular
  required += p.m_instances.size() * sizeof(GLfloat) * 16; // mat4:currModelTransform
  required += p.m_instances.size() * sizeof(GLfloat) * 16; // mat4:prevModelTransform
  required += p.m_instances.size() * sizeof(GLfloat) * 16; // mat4:normalTransform

  std::vector<float> instanceInfo;
  instanceInfo.reserve(required);
  for (OpenGLInstance *instance : p.m_instances)
  {
    KMatrix4x4 currTrans = currWorldToCamera * instance->currentTransform().toMatrix();
    KMatrix4x4 prevTrans = prevWorldToCamera * instance->previousTransform().toMatrix();
    KMatrix4x4 normTrans = currTrans.inverted().transposed();
    KColor const &diffuse  = instance->material().diffuse();
    KColor const &specular = instance->material().specularColor();
    float specularExponent = instance->material().specularExponent();
    instanceInfo.push_back( diffuse.redF() );
    instanceInfo.push_back( diffuse.greenF() );
    instanceInfo.push_back( diffuse.blueF() );
    instanceInfo.push_back( specular.redF() );
    instanceInfo.push_back( specular.greenF() );
    instanceInfo.push_back( specular.blueF() );
    instanceInfo.push_back( specularExponent );
    for (int i = 0; i < 16; ++i)
      instanceInfo.push_back( currTrans.constData()[i] );
    for (int i = 0; i < 16; ++i)
      instanceInfo.push_back( prevTrans.constData()[i] );
    for (int i = 0; i < 16; ++i)
      instanceInfo.push_back( normTrans.constData()[i] );
    instance->update();
  }

  // Prepare OpenGL Buffer data
  if (p.m_bufferSize < required)
  {
    p.m_buffer.bind();
    p.m_buffer.allocate(instanceInfo.data(), required);
    p.m_buffer.release();
    p.m_bufferSize = required;
  }
  else
  {
    p.m_buffer.bind();
    p.m_buffer.write(0, instanceInfo.data(), required);
    p.m_buffer.release();
  }
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
