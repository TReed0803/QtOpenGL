#ifndef OPENGLINSTANCEMANAGER_H
#define OPENGLINSTANCEMANAGER_H OpenGLInstanceManager

class OpenGLInstance;
class OpenGLViewport;
#include <KUniquePointer>

class OpenGLInstanceManagerPrivate;
class OpenGLInstanceManager
{
public:
  OpenGLInstanceManager();
  ~OpenGLInstanceManager();
  void create();
  void commit(const OpenGLViewport &view);
  void render() const;
  OpenGLInstance *createInstance();
private:
  KUniquePointer<OpenGLInstanceManagerPrivate> m_private;
};

#endif // OPENGLINSTANCEMANAGER_H
