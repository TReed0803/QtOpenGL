#include "khalfedgeobjparser.h"
#include "khalfedgemesh.h"

KHalfEdgeObjParser::KHalfEdgeObjParser(KHalfEdgeMesh *mesh, KAbstractReader *reader) :
  KAbstractObjParser(reader), m_mesh(mesh)
{
  // Intentionally Empty
}

void KHalfEdgeObjParser::onVertex(float vertex[4])
{
  KVector3D kvert(vertex[0], vertex[1], vertex[2]);
  m_mesh->addVertex(kvert);
}

void KHalfEdgeObjParser::onTexture(float texture[3])
{
  // Unsupported
  (void)texture;
}

void KHalfEdgeObjParser::onNormal(float normal[3])
{
  // Unsupported
  (void)normal;
}

void KHalfEdgeObjParser::onParameter(float parameter[3])
{
  // Unsupported
  (void)parameter;
}

void KHalfEdgeObjParser::onFace(index_array indices[], size_type count)
{
  // Calculate the average
  if (count > 3)
    triangulateFace(indices, count);
  else
    m_mesh->addFace(indices[0], indices[1], indices[2]);
}

void KHalfEdgeObjParser::triangulateFace(index_array indices[], KAbstractObjParser::size_type count)
{
  // Create the averaged vertex
  size_t i;
  KVector3D avg(m_mesh->vertex(indices[0][0])->position);
  for (i = 1; i < count; ++i)
  {
    avg += m_mesh->vertex(indices[i][0])->position;
  }
  avg /= count;
  index_array newVertex = { m_mesh->addVertex(avg), 0, 0 };

  // Add faces
  for (i = 1; i < count; ++i)
  {
    m_mesh->addFace(indices[i-1], indices[i], newVertex);
  }
  m_mesh->addFace(indices[i-1], indices[0], newVertex);
}

void KHalfEdgeObjParser::onGroup(char *group)
{
  // Unsupported
  (void)group;
}

void KHalfEdgeObjParser::onMaterial(char *file)
{
  // Unsupported
  (void)file;
}

void KHalfEdgeObjParser::onUseMaterial(char *mat)
{
  // Unsupported
  (void)mat;
}

void KHalfEdgeObjParser::onObject(char *obj)
{
  // Unsupported
  (void)obj;
}

void KHalfEdgeObjParser::onSmooth(char *smooth)
{
  // Unsupported
  (void)smooth;
}
