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
  void setValues(float A, float B, float C, float D, float E, float F, float W);
  void setExposureBias(float eb);
  void setExposure(float e);
private:
  ViewportPresentationPassPrivate *m_private;
};

#endif // VIEWPORTPRESENTATIONPASS_H
