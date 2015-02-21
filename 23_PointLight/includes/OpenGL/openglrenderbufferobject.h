#ifndef OPENGLRENDERBUFFEROBJECT_H
#define OPENGLRENDERBUFFEROBJECT_H

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
  void setStorage(OpenGLStorage storage);
  OpenGLStorage storage() const;
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
