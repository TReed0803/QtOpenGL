#ifndef OPENGLVIEWPORT_H
#define OPENGLVIEWPORT_H OpenGLViewport

class KCamera3D;
class OpenGLRenderBlock;
#include <KSharedPointer>

class OpenGLViewportPrivate;
class OpenGLViewport
{
public:
  OpenGLViewport();
  ~OpenGLViewport();
  void create();
  void bind();
  void release();
  void resize(int width, int height);
  void setCamera(const KCamera3D &camera);
  void setNearFar(float nearPlane, float farPlane);
  void setWidthHeight(int width, int height);
  void update();
  const OpenGLRenderBlock &current() const;
  const OpenGLRenderBlock &previous() const;
private:
  KSharedPointer<OpenGLViewportPrivate> m_private;
};

#endif // OPENGLVIEWPORT_H
