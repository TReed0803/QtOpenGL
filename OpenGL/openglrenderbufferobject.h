#ifndef OPENGLRENDERBUFFEROBJECT_H
#define OPENGLRENDERBUFFEROBJECT_H OpenGLRenderbufferObject

#include <OpenGLStorage>

class KSize;

class OpenGLRenderbufferObjectPrivate;
class OpenGLRenderbufferObject
{
public:
  OpenGLRenderbufferObject();
  ~OpenGLRenderbufferObject();

  void create();
  void bind();
  void release();
  void setInternalFormat(OpenGLInternalFormat storage);
  OpenGLInternalFormat internalFormat() const;
  OpenGLFormat format() const;
  OpenGLType texelType() const;
  void setSize(int width, int height);
  void setSize(KSize const &size);
  KSize const& size() const;
  int objectId();
  void allocate();

private:
  OpenGLRenderbufferObjectPrivate *m_private;
  void removeReference();
};

#endif // OPENGLRENDERBUFFEROBJECT_H
