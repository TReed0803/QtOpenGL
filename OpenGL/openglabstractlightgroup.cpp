#include "openglabstractlightgroup.h"

#include <OpenGLMesh>

void OpenGLAbstractLightGroup::setMesh(const KHalfEdgeMesh &mesh)
{
  OpenGLMesh glMesh;
  glMesh.create(mesh);
  setMesh(glMesh);
}
