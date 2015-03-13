#ifndef OPENGLRENDERVIEW_H
#define OPENGLRENDERVIEW_H OpenGLRenderView

#include <KSharedPointer>

class KSize;
class OpenGLRenderPassQueue;
class OpenGLViewport;
class OpenGLScene;

class OpenGLRenderViewPrivate;
class OpenGLRenderView
{
public:

  // Constructors / Destructor
  OpenGLRenderView();
  ~OpenGLRenderView();

  // Render Pass Queue
  void create(const OpenGLRenderPassQueue &queue, OpenGLViewport &view);
  void bind();
  void resize(const KSize &size);
  void resize(int width, int height);
  void commit(OpenGLScene &scene);
  void render(OpenGLScene &scene);
  void teardown();
  void release();

private:
  KSharedPointer<OpenGLRenderViewPrivate> m_private;
};

#endif // OPENGLRENDERVIEW_H
