#include "openglinstancegroup.h"
#include "openglbuffer.h"
#include "openglmesh.h"
#include "openglvertexarrayobject.h"
#include "openglfunctions.h"

#include <vector>

#include <KTransform3D>

///
class OpenGLInstanceGroupPrivate
{
public:
  typedef std::vector<KTransform3D> TransformList;

  OpenGLInstanceGroupPrivate();

  uint64_t m_bufferSize;
  OpenGLMesh *m_mesh;
  OpenGLBuffer m_buffer;
  TransformList m_transforms;
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
    f.glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 16, (void*)(sizeof(GLfloat) * i * 4));
    f.glVertexAttribDivisor(2 + i, 1);
  }
  obj->release();
}

void OpenGLInstanceGroup::pushTransform(const KTransform3D &t)
{
  P(OpenGLInstanceGroupPrivate);
  p.m_transforms.push_back(t);
}

void OpenGLInstanceGroup::updateBuffers()
{
  P(OpenGLInstanceGroupPrivate);

  std::vector<float> mat4Info;
  mat4Info.reserve(p.m_transforms.size() * 16);
  for (KTransform3D &t : p.m_transforms)
  {
    float const *m = t.toMatrix().data();
    for (int i = 0; i < 16; ++i)
      mat4Info.push_back( m[i] );
  }

  // Prepare OpenGL Buffer data
  uint64_t required = mat4Info.size() * sizeof(GLfloat);
  if (p.m_bufferSize < required)
  {
    p.m_buffer.bind();
    p.m_buffer.allocate(mat4Info.data(), required);
    p.m_buffer.release();
    p.m_bufferSize = required;
  }
  else
  {
    p.m_buffer.bind();
    p.m_buffer.write(0, mat4Info.data(), required);
    p.m_buffer.release();
  }
}

void OpenGLInstanceGroup::clear()
{
  P(OpenGLInstanceGroupPrivate);
  p.m_transforms.clear();
}

void OpenGLInstanceGroup::draw()
{
  OpenGLFunctions f;
  f.initializeOpenGLFunctions();
  P(OpenGLInstanceGroupPrivate);
  p.m_mesh->vertexArrayObject()->bind();
  f.glDrawArraysInstanced(p.m_mesh->mode(), 0, p.m_mesh->count(), p.m_transforms.size());
  p.m_mesh->vertexArrayObject()->release();
}

std::vector<KTransform3D> &OpenGLInstanceGroup::transforms()
{
  P(OpenGLInstanceGroupPrivate);
  return p.m_transforms;
}
