#ifndef LIGHTACCUMULATIONPASS_H
#define LIGHTACCUMULATIONPASS_H LightAccumulationPass

#include <RenderPasses>
#include <OpenGLRenderPass>

class OpenGLSpotLight;
class OpenGLPointLight;
class OpenGLDirectionLight;
class OpenGLSpotLightGroup;
class OpenGLPointLightGroup;
class OpenGLDirectionLightGroup;

class LightAccumulationPassPrivate;
class LightAccumulationPass : public OpenGLRenderPassImpl<LightAccumulationPass, LightAccumulationPassId>
{
public:
  LightAccumulationPass();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void commit(OpenGLViewport &view);
  virtual void render(OpenGLScene &scene);
  virtual void teardown();
private:
  LightAccumulationPassPrivate *m_private;
};

#endif // LIGHTACCUMULATIONPASS_H
