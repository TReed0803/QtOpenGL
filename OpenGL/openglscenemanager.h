#ifndef OPENGLSCENEMANAGER_H
#define OPENGLSCENEMANAGER_H OpenGLSceneManager

#include <KUniquePointer>
class OpenGLUpdateEvent;
class OpenGLScene;

class OpenGLSceneManagerPrivate;
class OpenGLSceneManager
{
public:
  OpenGLSceneManager();
  ~OpenGLSceneManager();
  OpenGLScene *currentScene();
  void setScene(OpenGLScene *scene);
  void pushScene(OpenGLScene *scene);
  void popScene();
  void update(OpenGLUpdateEvent *event);
  bool empty();
  bool activeScene();
private:
  KUniquePointer<OpenGLSceneManagerPrivate> m_private;
};

#endif // OPENGLSCENEMANAGER_H
