#ifndef   OPENGLINSTANCE_H
#define   OPENGLINSTANCE_H OpenGLInstance

class KTransform3D;
class OpenGLMaterial;
class KHalfEdgeMesh;
class OpenGLMesh;
#include <string>
#include <KAabbBoundingVolume>
class OpenGLViewport;

class OpenGLInstancePrivate;
class OpenGLInstance
{
public:
  OpenGLInstance();
  ~OpenGLInstance();

  // OpenGL
  void bind();
  void commit(OpenGLViewport const &viewport);
  void release();
  void draw();

  KTransform3D &transform();
  KTransform3D &currentTransform();
  KTransform3D &previousTransform();
  void setMesh(const OpenGLMesh &mesh);
  const OpenGLMesh &mesh() const;
  OpenGLMesh &mesh();
  void setMaterial(OpenGLMaterial const &mat);
  OpenGLMaterial &material();
  OpenGLMaterial const &material() const;
  void update();
  KAabbBoundingVolume aabb() const;
private:
  OpenGLInstancePrivate *m_private;
};

#endif // OPENGLINSTANCE_H
