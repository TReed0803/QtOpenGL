#include "openglabstractlightgroup.h"

#include <KHalfEdgeMesh>

void OpenGLAbstractLightGroup::setMesh(const OpenGLMesh &mesh)
{
  m_mesh = mesh;
  m_mesh.bind();
  prepMesh(m_mesh);
  m_mesh.release();
}

void OpenGLAbstractLightGroup::setMesh(const KHalfEdgeMesh &mesh)
{
  OpenGLMesh glMesh;
  glMesh.create(mesh);
  setMesh(glMesh);
}

void OpenGLAbstractLightGroup::setMesh(const char *filepath)
{
  KHalfEdgeMesh mesh;
  mesh.create(filepath);
  setMesh(mesh);
}
