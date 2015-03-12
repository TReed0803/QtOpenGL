#ifndef OPENGLRENDERPASS_H
#define OPENGLRENDERPASS_H OpenGLRenderPass

class OpenGLScene;
class OpenGLViewport;
class OpenGLRenderer;

class OpenGLRenderPass
{
public:
  virtual unsigned id() const = 0;
  virtual void initialize() = 0;
  virtual void resize(int width, int height) = 0;
  virtual void commit(OpenGLViewport &view) = 0;
  virtual void render(OpenGLScene &scene) = 0;
  virtual void teardown() = 0;
};

template <unsigned ID>
class OpenGLRenderPassImpl : public OpenGLRenderPass
{
public:
  virtual unsigned id() const;
  static unsigned passId();
};

template <unsigned ID>
unsigned OpenGLRenderPassImpl<ID>::id() const
{
  return ID;
}

template <unsigned ID>
unsigned OpenGLRenderPassImpl<ID>::passId()
{
  return ID;
}

#endif // OPENGLRENDERPASS_H
