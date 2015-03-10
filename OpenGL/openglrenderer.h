#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

class OpenGLRenderBlock;
class OpenGLRenderPass;
class OpenGLInstance;
class OpenGLPointLight;
class OpenGLSpotLight;
class OpenGLDirectionLight;
class OpenGLPointLightGroup;
class OpenGLSpotLightGroup;
class OpenGLDirectionLightGroup;

class OpenGLRendererPrivate;
class OpenGLRenderer
{
public:
  OpenGLRenderer();

  // Render Passes
  void create();
  void initialize();
  void resize(int width, int height);
  void update(OpenGLRenderBlock &current, OpenGLRenderBlock &previous);
  void render();
  void teardown();

  // Object Creation
  OpenGLInstance *createInstance();
  OpenGLPointLight *createPointLight();
  OpenGLSpotLight *createSpotLight();
  OpenGLDirectionLight *createDirectionLight();
  OpenGLPointLightGroup &pointLights();
  OpenGLSpotLightGroup &spotLights();
  OpenGLDirectionLightGroup &directionLights();

  // Object Manipulation
  void renderGeometry();
  void renderLights();
  void pause(bool p);
  bool isPaused() const;

  // Pass Manipulation
  template <typename T>
  T *addPass();
  template <typename T>
  T *pass();

private:
  OpenGLRenderPass *getPass(unsigned id);
  void addPass(OpenGLRenderPass *pass);
  OpenGLRendererPrivate *m_private;
};

template <typename T>
T *OpenGLRenderer::addPass()
{
  T *pass = new T;
  addPass(pass);
  return pass;
}

template <typename T>
T *OpenGLRenderer::pass()
{
  return getPass(T::passIdentifier());
}

#endif // OPENGLRENDERER_H
