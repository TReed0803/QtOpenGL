#ifndef OPENGLABSTRACTLIGHTGROUP_H
#define OPENGLABSTRACTLIGHTGROUP_H OpenGLAbstractLightGroup

class KHalfEdgeMesh;
class KMatrix4x4;
class OpenGLShaderProgram;
class OpenGLViewport;
class OpenGLScene;

#include <OpenGLMesh>
#include <OpenGLTexture>
#include <OpenGLFramebufferObject>

class OpenGLAbstractLightGroup
{
public:
  typedef unsigned char Byte;

  // Construction Routines
  bool create();

  // Properties
  void setMesh(const OpenGLMesh &mesh);
  void setMesh(const KHalfEdgeMesh &mesh);
  void setMesh(const char *filepath);

  // Virtual
  virtual void prepMesh(OpenGLMesh &mesh) = 0;
  virtual void commit(const OpenGLViewport &view) = 0;
  virtual void draw() = 0;
  virtual void drawShadowed(OpenGLScene &scene) = 0;

protected:
  OpenGLMesh m_mesh;
  OpenGLTexture m_shadowTexture, m_shadowDepth;
  OpenGLFramebufferObject m_shadowMappingFbo;
  OpenGLShaderProgram *m_regularLight;
  OpenGLShaderProgram *m_shadowCastingLight;
  OpenGLShaderProgram *m_shadowMappingLight;
};

#endif // OPENGLABSTRACTLIGHTGROUP_H
