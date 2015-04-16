#ifndef SCREENSPACEAMBIENTOCCLUSION_H
#define SCREENSPACEAMBIENTOCCLUSION_H ScreenSpaceAmbientOcclusion

#include <RenderPasses>
#include <OpenGLRenderPass>

class ScreenSpaceAmbientOcclusionPrivate;
class ScreenSpaceAmbientOcclusion : public OpenGLRenderPassImpl<ScreenSpaceAmbientOcclusion, ScreenSpaceAmbientOcclusionPassId>
{
public:
  ScreenSpaceAmbientOcclusion();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void commit(OpenGLViewport &view);
  virtual void render(OpenGLScene &scene);
  virtual void teardown();
private:
  ScreenSpaceAmbientOcclusionPrivate *m_private;
};

#endif // SCREENSPACEAMBIENTOCCLUSION_H
