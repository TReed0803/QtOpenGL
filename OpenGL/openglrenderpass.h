#ifndef OPENGLRENDERPASS_H
#define OPENGLRENDERPASS_H OpenGLRenderPass

class OpenGLRenderBlock;
class OpenGLRenderer;

class OpenGLRenderPass
{
public:
  virtual void initialize() = 0;
  virtual void resize(int width, int height) = 0;
  virtual void stage() = 0;
  virtual void commit(OpenGLRenderBlock &current, OpenGLRenderBlock &previous) = 0;
  virtual void render(OpenGLRenderer &renderer) = 0;
  virtual void teardown() = 0;
};

#endif // OPENGLRENDERPASS_H
