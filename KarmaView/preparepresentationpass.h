#ifndef PREPAREPRESENTATIONPASS_H
#define PREPAREPRESENTATIONPASS_H PreparePresentationPass

#include <RenderPasses>
#include <OpenGLRenderPass>

class PreparePresentationPassPrivate;
class PreparePresentationPass : public OpenGLRenderPassImpl<PreparePresentationPass, PreparePresentationPassId>
{
public:
  PreparePresentationPass();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void commit(OpenGLViewport &view);
  virtual void render(OpenGLScene &scene);
  virtual void teardown();
private:
  PreparePresentationPassPrivate *m_private;
};

#endif // PREPAREPRESENTATIONPASS_H
