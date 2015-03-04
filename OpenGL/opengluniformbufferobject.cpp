#include "opengluniformbufferobject.h"
#include <OpenGLFunctions>

class OpenGLUniformBufferObjectPrivate
{
public:
  int location;
  OpenGLFunctions f;
};


OpenGLUniformBufferObject::OpenGLUniformBufferObject() :
  OpenGLBuffer(OpenGLBuffer::UniformBuffer)
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
