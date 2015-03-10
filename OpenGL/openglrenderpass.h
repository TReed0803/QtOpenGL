#ifndef OPENGLRENDERPASS_H
#define OPENGLRENDERPASS_H OpenGLRenderPass

class OpenGLRenderBlock;
class OpenGLRenderer;

class OpenGLRenderPass
{
public:
  virtual unsigned id() const = 0;
  virtual void initialize() = 0;
  virtual void resize(int width, int height) = 0;
  virtual void commit(OpenGLRenderBlock &current, OpenGLRenderBlock &previous) = 0;
  virtual void render(OpenGLRenderer &renderer) = 0;
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
