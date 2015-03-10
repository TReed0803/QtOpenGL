#ifndef COMPOSITIONPASS_H
#define COMPOSITIONPASS_H CompositionPass

#include <RenderPasses>
#include <OpenGLRenderPass>

class CompositionPassPrivate;
class CompositionPass : public OpenGLRenderPassImpl<CompositionPassId>
{
public:
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void stage();
  virtual void commit(OpenGLRenderBlock &current, OpenGLRenderBlock &previous);
  virtual void render(OpenGLRenderer &renderer);
  virtual void teardown();
private:
  CompositionPassPrivate *m_private;
};

#endif // COMPOSITIONPASS_H
