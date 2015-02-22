#include "opengluniformbufferobject.h"
#include <OpenGLFunctions>

class OpenGLUniformBufferObjectPrivate
{
public:
  int location;
  OpenGLFunctions f;
};

#define P(c) c &p = *m_private
OpenGLUniformBufferObject::OpenGLUniformBufferObject() :
  OpenGLBuffer(static_cast<QOpenGLBuffer::Type>(GL_UNIFORM_BUFFER))
 ,m_private(new OpenGLUniformBufferObjectPrivate)
{
  P(OpenGLUniformBufferObjectPrivate);
  p.location = 0;
}

OpenGLUniformBufferObject::~OpenGLUniformBufferObject()
{
  delete m_private;
}

bool OpenGLUniformBufferObject::bind(int uniformIndex)
{
  P(OpenGLUniformBufferObjectPrivate);
  p.location = uniformIndex;
  return this->OpenGLBuffer::bind();
}

void OpenGLUniformBufferObject::release()
{
  P(OpenGLUniformBufferObjectPrivate);
  p.location = 0;
  this->OpenGLBuffer::release();
}

GLuint OpenGLUniformBufferObject::locationId() const
{
  P(OpenGLUniformBufferObjectPrivate);
  return p.location;
}
