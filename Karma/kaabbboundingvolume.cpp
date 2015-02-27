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

KAabbBoundingVolume::KAabbBoundingVolume(const KAabbBoundingVolume &a, const KAabbBoundingVolume &b) :
  m_private(new KAabbBoundingVolumePrivate)
{
  P(KAabbBoundingVolumePrivate);

  // Get new maximum
  if (a.extents().max.x() > b.extents().max.x())
    p.maxMin.max.setX(a.extents().max.x());
  else
    p.maxMin.max.setX(b.extents().max.x());

  if (a.extents().max.y() > b.extents().max.y())
    p.maxMin.max.setY(a.extents().max.y());
  else
    p.maxMin.max.setY(b.extents().max.y());

  if (a.extents().max.z() > b.extents().max.z())
    p.maxMin.max.setZ(a.extents().max.z());
  else
    p.maxMin.max.setZ(b.extents().max.z());

  // Get new minimum
  if (a.extents().min.x() < b.extents().min.x())
    p.maxMin.min.setX(a.extents().min.x());
  else
    p.maxMin.min.setX(b.extents().min.x());

  if (a.extents().min.y() < b.extents().min.y())
    p.maxMin.min.setY(a.extents().min.y());
  else
    p.maxMin.min.setY(b.extents().min.y());

  if (a.extents().min.z() < b.extents().min.z())
    p.maxMin.min.setZ(a.extents().min.z());
  else
    p.maxMin.min.setZ(b.extents().min.z());
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

Karma::MinMaxKVector3D const &KAabbBoundingVolume::extents() const
{
  P(KAabbBoundingVolumePrivate);
  return p.maxMin;
}

KVector3D const &KAabbBoundingVolume::minExtent() const
{
  P(KAabbBoundingVolumePrivate);
  return p.maxMin.min;
}

KVector3D const &KAabbBoundingVolume::maxExtent() const
{
  P(KAabbBoundingVolumePrivate);
  return p.maxMin.max;
}

KVector3D KAabbBoundingVolume::maxAxis() const
{
  P(KAabbBoundingVolumePrivate);
  KVector3D diff = p.maxMin.max - p.maxMin.min;
  if (diff.x() > diff.y())
  {
    if (diff.x() > diff.z())
      return KVector3D(1.0f, 0.0f, 0.0f);
    else
      return KVector3D(0.0f, 0.0f, 1.0f);
  }
  else
  {
    if (diff.y() > diff.z())
      return KVector3D(0.0f, 1.0f, 0.0f);
    else
      return KVector3D(0.0f, 0.0f, 1.0f);
  }
}

KVector3D KAabbBoundingVolume::minAxis() const
{
  P(KAabbBoundingVolumePrivate);
  KVector3D diff = p.maxMin.max - p.maxMin.min;
  if (diff.x() < diff.y())
  {
    if (diff.x() < diff.z())
      return KVector3D(1.0f, 0.0f, 0.0f);
    else
      return KVector3D(0.0f, 0.0f, 1.0f);
  }
  else
  {
    if (diff.y() < diff.z())
      return KVector3D(0.0f, 1.0f, 0.0f);
    else
      return KVector3D(0.0f, 0.0f, 1.0f);
  }
}

KVector3D KAabbBoundingVolume::center() const
{
  P(KAabbBoundingVolumePrivate);
  return (p.maxMin.max + p.maxMin.min) / 2.0f;
}

void KAabbBoundingVolume::setMinMaxBounds(const Karma::MinMaxKVector3D &minMax)
{
  P(KAabbBoundingVolumePrivate);
  p.maxMin = minMax;
}

void KAabbBoundingVolume::draw(KTransform3D &t, KColor const &color) const
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

void KAabbBoundingVolume::constructPrivate()
{
  m_private = new KAabbBoundingVolumePrivate;
}
