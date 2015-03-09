#ifndef LIGHTPASS_H
#define LIGHTPASS_H LightPass

#include <OpenGLRenderPass>

class OpenGLSpotLight;
class OpenGLPointLight;
class OpenGLDirectionLight;
class OpenGLSpotLightGroup;
class OpenGLPointLightGroup;
class OpenGLDirectionLightGroup;

class LightPassPrivate;
class LightPass : public OpenGLRenderPass
{
public:
  LightPass();
  virtual void initialize();
  virtual void resize(int width, int height);
  virtual void stage();
  virtual void commit(OpenGLRenderBlock &current, OpenGLRenderBlock &previous);
  virtual void render(OpenGLRenderer &renderer);
  virtual void teardown();

  // Temporary
  OpenGLSpotLightGroup &spotLights();
  OpenGLPointLightGroup &pointLights();
  OpenGLDirectionLightGroup &directionLights();
  OpenGLSpotLight *createSpotLight();
  OpenGLPointLight *createPointLight();
  OpenGLDirectionLight *createDirectionLight();
private:
  LightPassPrivate *m_private;
};

#endif // LIGHTPASS_H
