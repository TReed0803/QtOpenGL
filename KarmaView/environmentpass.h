#ifndef ENVIRONMENTPASS_H
#define ENVIRONMENTPASS_H EnvironmentPass

#include <RenderPasses>
#include <OpenGLRenderPass>

class EnvironmentPassPrivate;
class EnvironmentPass : public OpenGLRenderPassImpl<EnvironmentPass, EnvironmentPassId>
{
public:
  EnvironmentPass();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void commit(OpenGLViewport &view);
  virtual void render(OpenGLScene &scene);
  virtual void teardown();
private:
  EnvironmentPassPrivate *m_private;
};

#endif // ENVIRONMENTPASS_H
