#ifndef GBUFFERPASS_H
#define GBUFFERPASS_H GBufferPass

#include <OpenGLRenderPass>

class GBufferPassPrivate;
class GBufferPass : public OpenGLRenderPass
{
public:
  GBufferPass();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void stage();
  virtual void commit(OpenGLRenderBlock &current, OpenGLRenderBlock &previous);
  virtual void render(OpenGLRenderer &renderer);
  virtual void teardown();
private:
  GBufferPassPrivate *m_private;
};

#endif // GBUFFERPASS_H
