#ifndef OPENGLRENDERPASSQUEUE_H
#define OPENGLRENDERPASSQUEUE_H OpenGLRenderPassQueue

class KSize;
class OpenGLRenderPass;
class OpenGLScene;
class OpenGLViewport;

#include <vector>
#include <KSharedPointer>

class OpenGLRenderPassQueuePrivate;
class OpenGLRenderPassQueue
{
public:

  typedef std::vector<OpenGLRenderPass*> RenderPassContainer;
  typedef const std::vector<OpenGLRenderPass*> ConstRenderPassContainer;

  OpenGLRenderPassQueue();
  ~OpenGLRenderPassQueue();

  void create();
  void create(const OpenGLRenderPassQueue &queue);
  void resize(const KSize &size);
  void resize(int width, int height);
  void commit(OpenGLViewport &view);
  void render(OpenGLScene &scene);
  void teardown();

  OpenGLRenderPass *pass(int id);
  RenderPassContainer &passes();
  void addPass(OpenGLRenderPass *pass);
  ConstRenderPassContainer &passes() const;

private:
  KSharedPointer<OpenGLRenderPassQueuePrivate> m_private;
};

#endif // OPENGLRENDERPASSQUEUE_H
