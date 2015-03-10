#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H OpenGLRenderer

class KScene;
class OpenGLRenderBlock;
class OpenGLRenderPass;
class OpenGLViewport;

class OpenGLRendererPrivate;
class OpenGLRenderer
{
public:
  OpenGLRenderer();

  // Render Passes
  void create();
  void initialize();
  void resize(int width, int height);
  void commit(const OpenGLViewport &view);
  void render(const KScene &scene);
  void teardown();

  // Object Manipulation
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
