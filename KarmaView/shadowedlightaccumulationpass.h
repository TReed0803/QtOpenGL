#ifndef SHADOWEDLIGHTACCUMULATIONPASS_H
#define SHADOWEDLIGHTACCUMULATIONPASS_H ShadowedLightAccumulationPass

#include <RenderPasses>
#include <OpenGLRenderPass>

class ShadowedLightAccumulationPassPrivate;
class ShadowedLightAccumulationPass : public OpenGLRenderPassImpl<ShadowedLightAccumulationPass, ShadowedLightAccumulationPassId>
{
public:
  ShadowedLightAccumulationPass();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void commit(OpenGLViewport &view);
  virtual void render(OpenGLScene &scene);
  virtual void teardown();
private:
  ShadowedLightAccumulationPassPrivate *m_private;
};

#endif // SHADOWEDLIGHTACCUMULATIONPASS_H
