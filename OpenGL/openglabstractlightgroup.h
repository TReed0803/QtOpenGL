#ifndef OPENGLABSTRACTLIGHTGROUP_H
#define OPENGLABSTRACTLIGHTGROUP_H OpenGLAbstractLightGroup

class KHalfEdgeMesh;
class KMatrix4x4;

#include <OpenGLMesh>

class OpenGLAbstractLightGroup
{
public:
  // Properties
  void setMesh(const OpenGLMesh &mesh);
  void setMesh(const KHalfEdgeMesh &mesh);
  void setMesh(const char *filepath);

  // Virtual
  virtual void prepMesh(OpenGLMesh &mesh) = 0;
  virtual void update(const KMatrix4x4 &perspective, const KMatrix4x4 &view) = 0;
  virtual void draw() = 0;

protected:
  OpenGLMesh m_mesh;
};

#endif // OPENGLABSTRACTLIGHTGROUP_H
