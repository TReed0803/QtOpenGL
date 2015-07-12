#include "openglscenemanager.h"

#include <KMacros>
#include <KStack>
#include <OpenGLScene>

class OpenGLSceneManagerPrivate
{
public:
  OpenGLSceneManagerPrivate();
  OpenGLScene *m_currentActive;
  KStack<OpenGLScene*> m_sceneStack;
};

OpenGLSceneManagerPrivate::OpenGLSceneManagerPrivate() :
  m_currentActive(0)
{
  // Intentionally Empty
}

OpenGLSceneManager::OpenGLSceneManager() :
  m_private(new OpenGLSceneManagerPrivate)
{
  // Intentionally Empty
}

OpenGLSceneManager::~OpenGLSceneManager()
{
  // Intentionally Empty
}

OpenGLScene *OpenGLSceneManager::currentScene()
{
  P(OpenGLSceneManagerPrivate);
  return p.m_currentActive;
}

void OpenGLSceneManager::setScene(OpenGLScene *scene)
{
  P(OpenGLSceneManagerPrivate);
  p.m_sceneStack.front() = scene;
}

void OpenGLSceneManager::pushScene(OpenGLScene *scene)
{
  P(OpenGLSceneManagerPrivate);
  p.m_sceneStack.push(scene);
}

void OpenGLSceneManager::popScene()
{
  P(OpenGLSceneManagerPrivate);
  p.m_sceneStack.pop();
}

void OpenGLSceneManager::update(OpenGLUpdateEvent *event)
{
  P(OpenGLSceneManagerPrivate);
  OpenGLScene *nextScene = p.m_sceneStack.empty() ? 0 : p.m_sceneStack.front();

  // See if we should start the next scene
  if (nextScene != p.m_currentActive)
  {
    if (p.m_currentActive)
    {
      p.m_currentActive->end();
      delete p.m_currentActive;
    }
    p.m_currentActive = nextScene;
    if (p.m_currentActive)
    {
      p.m_currentActive->start();
    }
  }

  // Update the current scene
  if (p.m_currentActive)
  {
    p.m_currentActive->update(event);
  }
}

bool OpenGLSceneManager::empty()
{
  P(OpenGLSceneManagerPrivate);
  return p.m_sceneStack.empty();
}

bool OpenGLSceneManager::activeScene()
{
  P(OpenGLSceneManagerPrivate);
  return (p.m_currentActive != nullptr);
}
