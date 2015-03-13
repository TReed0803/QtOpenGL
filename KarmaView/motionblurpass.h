#ifndef MOTIONBLURPASS_H
#define MOTIONBLURPASS_H MotionBlurPass

#include <RenderPasses>
#include <OpenGLRenderPass>

class MotionBlurPassPrivate;
class MotionBlurPass : public OpenGLRenderPassImpl<MotionBlurPass, MotionBlurPassId>
{
public:
  MotionBlurPass();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void commit(OpenGLViewport &view);
  virtual void render(OpenGLScene &scene);
  virtual void teardown();
private:
  MotionBlurPassPrivate *m_private;
};

#endif // MOTIONBLURPASS_H
