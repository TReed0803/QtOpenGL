#ifndef OPENGLRENDERPASS_H
#define OPENGLRENDERPASS_H OpenGLRenderPass

class OpenGLScene;
class OpenGLViewport;
class OpenGLRenderer;

class OpenGLRenderPass
{
public:
  OpenGLRenderPass();
  virtual unsigned id() const = 0;
  virtual void initialize() = 0;
  virtual void resize(int width, int height) = 0;
  virtual void commit(OpenGLViewport &view) = 0;
  virtual void render(OpenGLScene &scene) = 0;
  virtual void teardown() = 0;
  virtual OpenGLRenderPass *clone() const = 0;
  void setActive(bool a);
  bool active() const;
private:
  bool m_active;
};

template <typename PASS, unsigned ID>
class OpenGLRenderPassImpl : public OpenGLRenderPass
{
public:
  virtual unsigned id() const;
  virtual OpenGLRenderPass *clone() const;
  static unsigned passId();
};

template <typename PASS, unsigned ID>
unsigned OpenGLRenderPassImpl<PASS, ID>::id() const
{
  return ID;
}

template <typename PASS, unsigned ID>
unsigned OpenGLRenderPassImpl<PASS, ID>::passId()
{
  return ID;
}

template <typename PASS, unsigned ID>
OpenGLRenderPass *OpenGLRenderPassImpl<PASS, ID>::clone() const
{
  return new PASS();
}

#endif // OPENGLRENDERPASS_H
