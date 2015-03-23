#include "kadaptiveoctree.h"

#include <KMacros>
#include <KMatrix4x4>
#include <KHalfEdgeMesh>
#include <KTransform3D>
#include <KColor>

/*******************************************************************************
 * KAdaptiveOctreePrivate
 ******************************************************************************/
class KAdaptiveOctreePrivate
{
public:

};

/*******************************************************************************
 * KAdaptiveOctree
 ******************************************************************************/
KAdaptiveOctree::KAdaptiveOctree() :
  m_private(new KAdaptiveOctreePrivate)
{
  // Intentionally Empty
}

KAdaptiveOctree::~KAdaptiveOctree()
{
  delete m_private;
}

void KAdaptiveOctree::addGeometry(const KHalfEdgeMesh &mesh, KTransform3D &transform)
{
  /*
  P(KAdaptiveOctreePrivate);
  size_t currOffset = p.m_pointCloud.size();

  // Transform and store all vertices
  KMatrix4x4 const &modelToWorld = transform.toMatrix();
  for (KHalfEdgeMesh::Vertex const & v: mesh.vertices())
  {
    KVector3D temp = modelToWorld * v.position;
    p.m_pointCloud.emplace_back(temp);
  }

  // Aggregate all of the triangles formed of the vertices
  KHalfEdgeMesh::HalfEdge const *hEdge[3];
  for (KHalfEdgeMesh::Face const & f: mesh.faces())
  {
    hEdge[0] = mesh.halfEdge(f.first);
    hEdge[1] = mesh.halfEdge(hEdge[0]->next);
    hEdge[2] = mesh.halfEdge(hEdge[1]->next);
    p.m_triangleCloud.emplace_back(hEdge[0]->to + currOffset, hEdge[1]->to + currOffset, hEdge[2]->to + currOffset);
  }
  */
}

bool KAdaptiveOctree::isDirty() const
{
  return true;
}

void KAdaptiveOctree::build(KAdaptiveOctree::TerminationPred pred)
{

}

void KAdaptiveOctree::clear()
{

}

void KAdaptiveOctree::draw(KTransform3D &trans, const KColor &color)
{

}

void KAdaptiveOctree::draw(KTransform3D &trans, const KColor &color, size_t min)
{

}

void KAdaptiveOctree::draw(KTransform3D &trans, const KColor &color, size_t min, size_t max)
{

}

size_t KAdaptiveOctree::depth()
{
  return 0;
}

