#ifndef DEBUGGBUFFERPASS_H
#define DEBUGGBUFFERPASS_H DebugGBufferPass
#include <RenderPasses>
#include <OpenGLRenderPass>

class DebugGBufferPassPrivate;
class DebugGBufferPass : public OpenGLRenderPassImpl<DebugGBufferPass, DebugGBufferPassId>
{
public:
  DebugGBufferPass();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void commit(OpenGLViewport &view);
  virtual void render(OpenGLScene &scene);
  virtual void teardown();
private:
  DebugGBufferPassPrivate *m_private;
};

#endif // DEBUGGBUFFERPASS_H
