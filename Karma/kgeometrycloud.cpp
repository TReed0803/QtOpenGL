#include "kgeometrycloud.h"

#include <KHalfEdgeMesh>
#include <KMacros>
#include <KMatrix4x4>
#include <KPointCloud>
#include <KTransform3D>
#include <KTriangleIndexCloud>

/*******************************************************************************
 * KGeometryCloudPrivate
 ******************************************************************************/
class KGeometryCloudPrivate
{
public:
  KPointCloud m_pointCloud;
  KTriangleIndexCloud m_triangleCloud;
};

/*******************************************************************************
 * KGeometryCloud
 ******************************************************************************/
KGeometryCloud::KGeometryCloud() :
  m_private(new KGeometryCloudPrivate)
{
  // Intentionally Empty
}

KGeometryCloud::~KGeometryCloud()
{
  // Intentionally Empty
}

void KGeometryCloud::addGeometry(const KHalfEdgeMesh &mesh)
{
  // Identity Transform3D will apply no change.
  addGeometry(mesh, KTransform3D());
}

void KGeometryCloud::addGeometry(const KHalfEdgeMesh &mesh, const KTransform3D &trans)
{
  P(KGeometryCloudPrivate);
  size_t currOffset = p.m_pointCloud.size();

  // Add points to point cloud
  p.m_pointCloud.reserve(p.m_pointCloud.size() + mesh.vertices().size());
  KMatrix4x4 const &modelToWorld = trans.toMatrix();
  for (KHalfEdgeMesh::Vertex const & v: mesh.vertices())
  {
    p.m_pointCloud.emplace_back(modelToWorld * v.position);
  }

  // Add triangles to triangle cloud
  KTriangleIndexCloud::ElementType triangle;
  p.m_triangleCloud.reserve(p.m_triangleCloud.size() + mesh.faces().size());
  for (KHalfEdgeMesh::Face const & f: mesh.faces())
  {
    triangle.indices[0] = f.first;
    triangle.indices[1] = mesh.unsafeHalfEdge(triangle.indices[0])->next;
    triangle.indices[2] = mesh.unsafeHalfEdge(triangle.indices[1])->next;
    triangle.indices[0] = mesh.unsafeHalfEdge(triangle.indices[0])->to;
    triangle.indices[1] = mesh.unsafeHalfEdge(triangle.indices[1])->to;
    triangle.indices[2] = mesh.unsafeHalfEdge(triangle.indices[2])->to;
    p.m_triangleCloud.emplace_back(triangle.offset(currOffset));
  }
}

void KGeometryCloud::build(KGeometryCloud::BuildMethod method, KGeometryCloud::TerminationPred pred)
{
  // Do nothing
}

void KGeometryCloud::clear()
{
  m_private = new KGeometryCloudPrivate;
}

bool KGeometryCloud::dirty() const
{
  P(const KGeometryCloudPrivate);
  return (!p.m_pointCloud.empty() || !p.m_triangleCloud.empty());
}

const KPointCloud &KGeometryCloud::pointCloud() const
{
  P(const KGeometryCloudPrivate);
  return p.m_pointCloud;
}

const KTriangleIndexCloud &KGeometryCloud::triangleIndexCloud() const
{
  P(const KGeometryCloudPrivate);
  return p.m_triangleCloud;
}

KPointCloud &KGeometryCloud::pointCloud()
{
  P(KGeometryCloudPrivate);
  return p.m_pointCloud;
}

KTriangleIndexCloud &KGeometryCloud::triangleIndexCloud()
{
  P(KGeometryCloudPrivate);
  return p.m_triangleCloud;
}
