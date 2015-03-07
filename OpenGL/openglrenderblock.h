#ifndef OPENGLRENDERBLOCK_H
#define OPENGLRENDERBLOCK_H OpenGLRenderBlock

#include <KUniquePointer>
#include <OpenGLUniformBufferObject>

class KMatrix4x4;

class OpenGLRenderBlockPrivate;
class OpenGLRenderBlock : public OpenGLUniformBufferObject
{
public:

  // Constructors / Destructor
  OpenGLRenderBlock();
  ~OpenGLRenderBlock();

  // Render Block Properties
  void setViewMatrix(const KMatrix4x4 &view);
  void setPerspectiveMatrix(const KMatrix4x4 &perspective);
  void setNearFar(float nearPlane, float farPlane);
  void setDimensions(int width, int height);

  // Public Methods
  void update();
  void allocate();
  bool dirty() const;

private:
  KUniquePointer<OpenGLRenderBlockPrivate> m_private;
};

#endif // OPENGLRENDERBLOCK_H
