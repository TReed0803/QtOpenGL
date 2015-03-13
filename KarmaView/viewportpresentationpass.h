#ifndef VIEWPORTPRESENTATIONPASS_H
#define VIEWPORTPRESENTATIONPASS_H ViewportPresentationPass

#include <RenderPasses>
#include <OpenGLRenderPass>

class ViewportPresentationPassPrivate;
class ViewportPresentationPass : public OpenGLRenderPassImpl<ViewportPresentationPass, ViewportPresentationPassId>
{
public:
  ViewportPresentationPass();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void commit(OpenGLViewport &view);
  virtual void render(OpenGLScene &scene);
  virtual void teardown();
private:
  ViewportPresentationPassPrivate *m_private;
};

#endif // VIEWPORTPRESENTATIONPASS_H
