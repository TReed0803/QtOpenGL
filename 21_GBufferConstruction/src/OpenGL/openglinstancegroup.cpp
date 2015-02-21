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
};

OpenGLInstanceGroupPrivate::OpenGLInstanceGroupPrivate() :
  m_bufferSize(0)
{
  m_buffer.setUsagePattern(OpenGLBuffer::DynamicDraw);
  m_buffer.create();
}

///
#define P(c) c &p = *m_private
OpenGLInstanceGroup::OpenGLInstanceGroup(QObject *parent) :
  QObject(parent), m_private(new OpenGLInstanceGroupPrivate)
{
  // Intentionally Empty
}

void OpenGLInstanceGroup::setMesh(OpenGLMesh *mesh)
{
  P(OpenGLInstanceGroupPrivate);
  OpenGLFunctions f;
  f.initializeOpenGLFunctions();
  p.m_mesh = mesh;
  OpenGLVertexArrayObject *obj = p.m_mesh->vertexArrayObject();
  obj->bind();
  p.m_buffer.bind();
  for (int i = 0; i < 4; ++i)
  {
    f.glEnableVertexAttribArray(2 + i);
    f.glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, OpenGLInstance::Stride(), (void*)(OpenGLInstance::CurrentTransformOffset() + sizeof(GLfloat) * i * 4));
    f.glVertexAttribDivisor(2 + i, 1);
  }
  for (int i = 0; i < 4; ++i)
  {
    f.glEnableVertexAttribArray(6 + i);
    f.glVertexAttribPointer(6 + i, 4, GL_FLOAT, GL_FALSE, OpenGLInstance::Stride(), (void*)(OpenGLInstance::PreviousTransformOffset() + sizeof(GLfloat) * i * 4));
    f.glVertexAttribDivisor(6 + i, 1);
  }
  f.glEnableVertexAttribArray(10);
  f.glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, OpenGLInstance::Stride(), (void*)(OpenGLInstance::DiffuseOffset()));
  f.glVertexAttribDivisor(10, 1);
  f.glEnableVertexAttribArray(11);
  f.glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, OpenGLInstance::Stride(), (void*)(OpenGLInstance::SpecularOffset()));
  f.glVertexAttribDivisor(11, 1);
  obj->release();
}

void OpenGLInstanceGroup::update()
{
  P(OpenGLInstanceGroupPrivate);
  uint64_t required = 0;
  required += p.m_instances.size() * sizeof(GLfloat) * 16; // mat4:prev
  required += p.m_instances.size() * sizeof(GLfloat) * 16; // mat4:curr
  required += p.m_instances.size() * sizeof(GLfloat) * 3;  // vec3:diffuse
  required += p.m_instances.size() * sizeof(GLfloat) * 3;  // vec3:specularColor
  required += p.m_instances.size() * sizeof(GLfloat) * 1;  // float:specularExponent

  std::vector<float> instanceInfo;
  instanceInfo.reserve(required);
  for (OpenGLInstance *instance : p.m_instances)
  {
    float const *currTrans = instance->currentTransform().toMatrix().constData();
    float const *prevTrans = instance->previousTransform().toMatrix().constData();
    KColor const &diffuse  = instance->material().diffuse();
    KColor const &specular = instance->material().specularColor();
    float specularExponent = instance->material().specularExponent();
    for (int i = 0; i < 16; ++i)
      instanceInfo.push_back( currTrans[i] );
    for (int i = 0; i < 16; ++i)
      instanceInfo.push_back( prevTrans[i] );
    instanceInfo.push_back( diffuse.redF() );
    instanceInfo.push_back( diffuse.greenF() );
    instanceInfo.push_back( diffuse.blueF() );
    instanceInfo.push_back( specular.redF() );
    instanceInfo.push_back( specular.greenF() );
    instanceInfo.push_back( specular.blueF() );
    instanceInfo.push_back( specularExponent );
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
  OpenGLFunctions f;
  f.initializeOpenGLFunctions();
  P(OpenGLInstanceGroupPrivate);
  p.m_mesh->vertexArrayObject()->bind();
  f.glDrawElementsInstanced(p.m_mesh->mode(), p.m_mesh->count(), GL_UNSIGNED_INT, (void*)0, p.m_instances.size());
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
