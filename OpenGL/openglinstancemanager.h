#ifndef OPENGLINSTANCEMANAGER_H
#define OPENGLINSTANCEMANAGER_H OpenGLInstanceManager

class OpenGLInstance;
class OpenGLRenderBlock;
#include <KUniquePointer>

class OpenGLInstanceManagerPrivate;
class OpenGLInstanceManager
{
public:
  OpenGLInstanceManager();
  ~OpenGLInstanceManager();
  void create();
  void update(OpenGLRenderBlock &current, OpenGLRenderBlock &previous);
  void render();
  OpenGLInstance *createInstance();
private:
  KUniquePointer<OpenGLInstanceManagerPrivate> m_private;
};

#endif // OPENGLINSTANCEMANAGER_H
