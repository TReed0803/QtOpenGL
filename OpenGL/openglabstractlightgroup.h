#ifndef OPENGLABSTRACTLIGHTGROUP_H
#define OPENGLABSTRACTLIGHTGROUP_H OpenGLAbstractLightGroup

class KHalfEdgeMesh;
class KMatrix4x4;
class OpenGLRenderBlock;
class OpenGLShaderProgram;

#include <OpenGLMesh>

class OpenGLAbstractLightGroup
{
public:
  typedef unsigned char Byte;

  // Properties
  void setMesh(const OpenGLMesh &mesh);
  void setMesh(const KHalfEdgeMesh &mesh);
  void setMesh(const char *filepath);

  // Virtual
  virtual void prepMesh(OpenGLMesh &mesh) = 0;
  virtual void update(const OpenGLRenderBlock &stats) = 0;
  virtual void draw() = 0;

protected:
  OpenGLMesh m_mesh;
  OpenGLShaderProgram *m_regularLight;
  OpenGLShaderProgram *m_shadowCastingLight;
};

#endif // OPENGLABSTRACTLIGHTGROUP_H
