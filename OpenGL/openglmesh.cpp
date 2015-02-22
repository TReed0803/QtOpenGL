#include "openglmesh.h"
#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include <OpenGLVertexArrayObject>

class OpenGLMeshPrivate
{
public:
  OpenGLMeshPrivate();
  void drawElements();
  typedef void (OpenGLMeshPrivate::*DrawMethodCB)();
  OpenGLMesh::OpenGLBufferList m_buffers;
  OpenGLVertexArrayObject *m_vao;
  int m_count;
  GLenum m_mode;
  DrawMethodCB m_drawMethod;
  OpenGLFunctions f;
};

///

OpenGLMesh::OpenGLMesh(QObject *parent) : QObject(parent), m_private(new OpenGLMeshPrivate)
{
  P(OpenGLMeshPrivate);
  p.f.initializeOpenGLFunctions();
}

const OpenGLMesh::OpenGLBufferList &OpenGLMesh::getBuffers() const
{
  P(OpenGLMeshPrivate);
  return p.m_buffers;
}

OpenGLVertexArrayObject *OpenGLMesh::vertexArrayObject()
{
  P(OpenGLMeshPrivate);
  if (!p.m_vao)
  {
    p.m_vao = new OpenGLVertexArrayObject(this);
    p.m_vao->create();
  }
  return p.m_vao;
}

OpenGLBuffer OpenGLMesh::createBuffer(OpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern hint)
{
  P(OpenGLMeshPrivate);
  OpenGLBuffer buffer(type);
  buffer.create();
  buffer.setUsagePattern(hint);
  p.m_buffers.push_back(buffer);
  return buffer;
}

void OpenGLMesh::setMode(OpenGLMesh::Options options)
{
  (void)options;
}

void OpenGLMesh::setDrawArrays(GLenum mode, int count)
{
  P(OpenGLMeshPrivate);
  p.m_mode = mode;
  p.m_count = count;
  p.m_drawMethod = &OpenGLMeshPrivate::drawElements;
}

void OpenGLMesh::draw()
{
  P(OpenGLMeshPrivate);
  p.m_vao->bind();
  (p.*p.m_drawMethod)();
  p.m_vao->release();
}

GLenum OpenGLMesh::mode() const
{
  P(OpenGLMeshPrivate);
  return p.m_mode;
}

uint64_t OpenGLMesh::count() const
{
  P(OpenGLMeshPrivate);
  return p.m_count;
}

OpenGLMeshPrivate::OpenGLMeshPrivate()
{
  m_vao = Q_NULLPTR;
}

void OpenGLMeshPrivate::drawElements()
{
  f.glDrawElements(m_mode, m_count, GL_UNSIGNED_INT, (void*)0);
}
