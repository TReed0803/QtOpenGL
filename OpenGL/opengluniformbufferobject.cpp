#include "opengluniformbufferobject.h"
#include <OpenGLFunctions>

static GLuint sg_bindings[GL_MAX_UNIFORM_BUFFER_BINDINGS] = { 0 };

class OpenGLUniformBufferObjectPrivate : public OpenGLFunctions
{
public:
  OpenGLUniformBufferObjectPrivate();
};

OpenGLUniformBufferObjectPrivate::OpenGLUniformBufferObjectPrivate()
{
  initializeOpenGLFunctions();
}

OpenGLUniformBufferObject::OpenGLUniformBufferObject() :
  OpenGLBuffer(OpenGLBuffer::UniformBuffer)
 ,m_private(new OpenGLUniformBufferObjectPrivate)
{
  // Intentionally Empty
}

OpenGLUniformBufferObject::~OpenGLUniformBufferObject()
{
  delete m_private;
}

void OpenGLUniformBufferObject::bindBase(unsigned uniformIndex)
{
  P(OpenGLUniformBufferObjectPrivate);
  sg_bindings[uniformIndex] = bufferId();
  p.glBindBufferBase(GL_UNIFORM_BUFFER, uniformIndex, sg_bindings[uniformIndex]);
}

void OpenGLUniformBufferObject::release()
{
  this->OpenGLBuffer::release();
}

GLuint OpenGLUniformBufferObject::boundBufferId(unsigned index)
{
  return sg_bindings[index];
}

void OpenGLUniformBufferObject::bindBufferId(unsigned index, GLuint buffer)
{
  sg_bindings[index] = buffer;
}

int OpenGLUniformBufferObject::getAlignment()
{
  GLint offset;
  GL::glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offset);
  return int(offset);
}

int OpenGLUniformBufferObject::alignmentOffset()
{
  const static int alignmentOffset = getAlignment();
  return alignmentOffset;
}
