#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H OpenGLRenderer

class OpenGLScene;
class OpenGLRenderBlock;
class OpenGLRenderPass;
class OpenGLViewport;

class OpenGLRendererPrivate;
class OpenGLRenderer
{
public:
  OpenGLRenderer();

  // Render Passes
  void bind();
  void create();
  void resize(int width, int height);
  void render(OpenGLScene &scene);
  void teardown();
  void release();

  // Object Manipulation
  void pause(bool p);
  bool isPaused() const;

  // Pass Manipulation
  template <typename T>
  T *addPass();
  template <typename T>
  T *pass();

  OpenGLRenderPass *pass(int id);
  void registerViewport(OpenGLViewport *view);
  static void activateViewport(OpenGLViewport *view);

private:
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
  return pass(T::passId());
}

#endif // OPENGLRENDERER_H
