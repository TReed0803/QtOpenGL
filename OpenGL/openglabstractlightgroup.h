#ifndef OPENGLABSTRACTLIGHTGROUP_H
#define OPENGLABSTRACTLIGHTGROUP_H OpenGLAbstractLightGroup

class OpenGLMesh;
class KHalfEdgeMesh;
class KMatrix4x4;

class OpenGLAbstractLightGroup
{
public:
  // Properties
  void setMesh(const KHalfEdgeMesh &mesh);

  // Virtual
  virtual void setMesh(const OpenGLMesh &mesh) = 0;
  virtual void update(const KMatrix4x4 &perspective, const KMatrix4x4 &view) = 0;
  virtual void draw() = 0;

};

#endif // OPENGLABSTRACTLIGHTGROUP_H
