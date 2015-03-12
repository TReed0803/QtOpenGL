#ifndef LIGHTPASS_H
#define LIGHTPASS_H LightPass

#include <RenderPasses>
#include <OpenGLRenderPass>

class OpenGLSpotLight;
class OpenGLPointLight;
class OpenGLDirectionLight;
class OpenGLSpotLightGroup;
class OpenGLPointLightGroup;
class OpenGLDirectionLightGroup;

class LightPassPrivate;
class LightPass : public OpenGLRenderPassImpl<LightAccumulationPassId>
{
public:
  LightPass();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void commit(OpenGLViewport &view);
  virtual void render(OpenGLScene &scene);
  virtual void teardown();
private:
  LightPassPrivate *m_private;
};

#endif // LIGHTPASS_H
