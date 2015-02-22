#include "openglrenderbufferobject.h"

#include <KSize>
#include <QtOpenGL/QGL>
#include <OpenGLFunctions>

class OpenGLRenderbufferObjectPrivate
{
public:
  OpenGLRenderbufferObjectPrivate();
  ~OpenGLRenderbufferObjectPrivate();
  int m_references;
  GLuint m_objectId;
  OpenGLFunctions m_functions;
  OpenGLStorage m_storage;
  KSize m_size;
};

OpenGLRenderbufferObjectPrivate::OpenGLRenderbufferObjectPrivate() :
  m_references(1), m_objectId(0), m_storage(OpenGLStorage::Rgba8)
{
  m_functions.initializeOpenGLFunctions();
  m_functions.glGenRenderbuffers(1, &m_objectId);
}

OpenGLRenderbufferObjectPrivate::~OpenGLRenderbufferObjectPrivate()
{
  m_functions.glDeleteRenderbuffers(1, &m_objectId);
}


OpenGLRenderbufferObject::OpenGLRenderbufferObject() :
  m_private(0)
{
  // Intentionally Empty
}

OpenGLRenderbufferObject::~OpenGLRenderbufferObject()
{
  removeReference();
}

void OpenGLRenderbufferObject::create()
{
  removeReference();
  m_private = new OpenGLRenderbufferObjectPrivate;
}

void OpenGLRenderbufferObject::bind()
{
  P(OpenGLRenderbufferObjectPrivate);
  p.m_functions.glBindRenderbuffer(GL_RENDERBUFFER, p.m_objectId);
}

void OpenGLRenderbufferObject::release()
{
  P(OpenGLRenderbufferObjectPrivate);
  p.m_functions.glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void OpenGLRenderbufferObject::setStorage(OpenGLStorage storage)
{
  P(OpenGLRenderbufferObjectPrivate);
  p.m_storage = storage;
}

OpenGLStorage OpenGLRenderbufferObject::storage() const
{
  P(OpenGLRenderbufferObjectPrivate);
  return p.m_storage;
}

void OpenGLRenderbufferObject::setSize(int width, int height)
{
  P(OpenGLRenderbufferObjectPrivate);
  p.m_size.setWidth(width);
  p.m_size.setHeight(height);
}

void OpenGLRenderbufferObject::setSize(const KSize &size)
{
  setSize(size.width(), size.height());
}

const KSize &OpenGLRenderbufferObject::size() const
{
  P(OpenGLRenderbufferObjectPrivate);
  return p.m_size;
}

int OpenGLRenderbufferObject::objectId()
{
  P(OpenGLRenderbufferObjectPrivate);
  return p.m_objectId;
}

void OpenGLRenderbufferObject::allocate()
{
  P(OpenGLRenderbufferObjectPrivate);
  p.m_functions.glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)p.m_storage, p.m_size.width(), p.m_size.height());
}

void OpenGLRenderbufferObject::removeReference()
{
  if (m_private)
  {
    P(OpenGLRenderbufferObjectPrivate);
    --p.m_references;
    if (p.m_references == 0)
    {
      delete m_private;
      m_private = 0;
    }
  }
}

