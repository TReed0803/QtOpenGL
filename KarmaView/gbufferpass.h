#ifndef GBUFFERPASS_H
#define GBUFFERPASS_H GBufferPass

#include <RenderPasses>
#include <OpenGLRenderPass>

class GBufferPassPrivate;
class GBufferPass : public OpenGLRenderPassImpl<GBufferPassId>
{
public:
  GBufferPass();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void commit(OpenGLViewport &view);
  virtual void render(OpenGLScene &scene);
  virtual void teardown();
private:
  GBufferPassPrivate *m_private;
};

#endif // GBUFFERPASS_H
