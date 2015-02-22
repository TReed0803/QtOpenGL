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
  OpenGLMesh *m_mesh;
  OpenGLBuffer m_buffer;
  TransformList m_instances;
  OpenGLFunctions m_functions;
};

OpenGLInstanceGroupPrivate::OpenGLInstanceGroupPrivate() :
  m_bufferSize(0)
{
  m_functions.initializeOpenGLFunctions();
  m_buffer.setUsagePattern(OpenGLBuffer::DynamicDraw);
  m_buffer.create();
}

///

OpenGLInstanceGroup::OpenGLInstanceGroup(QObject *parent) :
  QObject(parent), m_private(new OpenGLInstanceGroupPrivate)
{
  // Intentionally Empty
}

void OpenGLInstanceGroup::setMesh(OpenGLMesh *mesh)
{
  P(OpenGLInstanceGroupPrivate);
  p.m_mesh = mesh;
  OpenGLVertexArrayObject *obj = p.m_mesh->vertexArrayObject();
  obj->bind();
  p.m_buffer.bind();
  p.m_functions.glEnableVertexAttribArray(2);
  p.m_functions.glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, OpenGLInstance::Stride(), (void*)(OpenGLInstance::DiffuseOffset()));
  p.m_functions.glVertexAttribDivisor(2, 1);
  p.m_functions.glEnableVertexAttribArray(3);
  p.m_functions.glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, OpenGLInstance::Stride(), (void*)(OpenGLInstance::SpecularOffset()));
  p.m_functions.glVertexAttribDivisor(3, 1);
  for (int i = 0; i < 4; ++i)
  {
    p.m_functions.glEnableVertexAttribArray(4 + i);
    p.m_functions.glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, OpenGLInstance::Stride(), (void*)(OpenGLInstance::CurrentTransformOffset() + sizeof(GLfloat) * i * 4));
    p.m_functions.glVertexAttribDivisor(4 + i, 1);
  }
  for (int i = 0; i < 4; ++i)
  {
    p.m_functions.glEnableVertexAttribArray(8 + i);
    p.m_functions.glVertexAttribPointer(8 + i, 4, GL_FLOAT, GL_FALSE, OpenGLInstance::Stride(), (void*)(OpenGLInstance::PreviousTransformOffset() + sizeof(GLfloat) * i * 4));
    p.m_functions.glVertexAttribDivisor(8 + i, 1);
  }
  for (int i = 0; i < 4; ++i)
  {
    p.m_functions.glEnableVertexAttribArray(12 + i);
    p.m_functions.glVertexAttribPointer(12 + i, 4, GL_FLOAT, GL_FALSE, OpenGLInstance::Stride(), (void*)(OpenGLInstance::NormalsTransformOffset() + sizeof(GLfloat) * i * 4));
    p.m_functions.glVertexAttribDivisor(12 + i, 1);
  }
  obj->release();
}

void OpenGLInstanceGroup::update(KMatrix4x4 const &currWorldToCamera, KMatrix4x4 const &prevWorldToCamera)
{
  P(OpenGLInstanceGroupPrivate);
  uint64_t required = 0;
  required += p.m_instances.size() * sizeof(GLfloat) * 3;  // vec3:diffuse
  required += p.m_instances.size() * sizeof(GLfloat) * 4;  // vec3:specular
  required += p.m_instances.size() * sizeof(GLfloat) * 16; // mat4:prevModelTransform
  required += p.m_instances.size() * sizeof(GLfloat) * 16; // mat4:currModelTransform
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
  p.m_mesh->vertexArrayObject()->bind();
  p.m_functions.glDrawElementsInstanced(p.m_mesh->mode(), p.m_mesh->count(), GL_UNSIGNED_INT, (void*)0, p.m_instances.size());
  p.m_mesh->vertexArrayObject()->release();
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
