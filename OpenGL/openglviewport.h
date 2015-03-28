#ifndef OPENGLVIEWPORT_H
#define OPENGLVIEWPORT_H OpenGLViewport

class KCamera3D;
class KSize;
class KSizeF;
class KVector2D;
class OpenGLRenderBlock;
class KFrustum;
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
  void resize(const KSize &size);
  void setCamera(const KCamera3D &camera);
  void setNearFar(float nearPlane, float farPlane);
  void setRegion(float x, float y, float w, float h);
  const KSize &size() const;
  const KSize &screenSize() const;
  const KVector2D &origin() const;
  const KSizeF &region() const;
  const KFrustum &frustum() const;
  void commit();
  void activate();
  const OpenGLRenderBlock &current() const;
  const OpenGLRenderBlock &previous() const;
private:
  KSharedPointer<OpenGLViewportPrivate> m_private;
};

#endif // OPENGLVIEWPORT_H
