#include "kaabbboundingvolume.h"
#include <KMacros>
#include <KHalfEdgeMesh>
#include <OpenGLDebugDraw>
#include <KTransform3D>
#include <KMatrix3x3>
#include <KMath>

class KAabbBoundingVolumePrivate
{
public:
  void calculateMinMaxMethod(KHalfEdgeMesh const &mesh);
  Karma::MinMaxKVector3D maxMin;
};

void KAabbBoundingVolumePrivate::calculateMinMaxMethod(const KHalfEdgeMesh &mesh)
{
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();
  maxMin = Karma::findMinMaxBounds(vertices.begin(), vertices.end(), KHalfEdgeMesh::VertexPositionPred());
}

KAabbBoundingVolume::KAabbBoundingVolume(KHalfEdgeMesh const &mesh, Method method) :
  m_private(new KAabbBoundingVolumePrivate)
{
  P(KAabbBoundingVolumePrivate);
  switch (method)
  {
  case MinMaxMethod:
    p.calculateMinMaxMethod(mesh);
    break;
  }
}

KAabbBoundingVolume::~KAabbBoundingVolume()
{
  delete m_private;
}

void KAabbBoundingVolume::draw(KTransform3D &t, KColor const &color)
{
  P(KAabbBoundingVolumePrivate);
  KMatrix4x4 const &mtx = t.toMatrix();

  // Construct translated pointset
  std::vector<KVector3D> tVec =
  {
    mtx * p.maxMin.min,
    mtx * p.maxMin.max,
    mtx * KVector3D( p.maxMin.min.x(), p.maxMin.max.y(), p.maxMin.max.z()),
    mtx * KVector3D( p.maxMin.min.x(), p.maxMin.min.y(), p.maxMin.max.z()),
    mtx * KVector3D( p.maxMin.max.x(), p.maxMin.min.y(), p.maxMin.max.z()),
    mtx * KVector3D( p.maxMin.max.x(), p.maxMin.min.y(), p.maxMin.min.z()),
    mtx * KVector3D( p.maxMin.max.x(), p.maxMin.max.y(), p.maxMin.min.z()),
    mtx * KVector3D( p.maxMin.min.x(), p.maxMin.max.y(), p.maxMin.min.z())
  };

  // Find and draw the Aabb of the translated pointset
  Karma::MinMaxKVector3D mm = Karma::findMinMaxBounds(tVec.begin(), tVec.end());
  OpenGLDebugDraw::World::drawAabb(mm.min, mm.max, color);
}
