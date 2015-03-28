#include "kaabbboundingvolume.h"
#include <KMacros>
#include <KHalfEdgeMesh>
#include <OpenGLDebugDraw>
#include <KTransform3D>
#include <KMatrix3x3>
#include <KMatrix4x4>
#include <KMath>

class KAabbBoundingVolumePrivate
{
public:
  KAabbBoundingVolumePrivate();
  void calculateMinMaxMethod(KHalfEdgeMesh const &mesh);
  Karma::MinMaxKVector3D maxMin;
};

KAabbBoundingVolumePrivate::KAabbBoundingVolumePrivate() :
  maxMin(std::numeric_limits<float>::min(), std::numeric_limits<float>::max())
{
  // Intentionally Empty
}

void KAabbBoundingVolumePrivate::calculateMinMaxMethod(const KHalfEdgeMesh &mesh)
{
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();
  maxMin = Karma::findMinMaxBounds(vertices.begin(), vertices.end(), KHalfEdgeMesh::VertexPositionPred());
}

KAabbBoundingVolume::KAabbBoundingVolume() :
  m_private(new KAabbBoundingVolumePrivate)
{
  // Intentionally Empty
}

KAabbBoundingVolume::KAabbBoundingVolume(const KAabbBoundingVolume &rhs) :
  m_private(new KAabbBoundingVolumePrivate)
{
  P(KAabbBoundingVolumePrivate);
  p.maxMin = rhs.m_private->maxMin;
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

KAabbBoundingVolume::KAabbBoundingVolume(const KAabbBoundingVolume &a, const KVector3D &offset) :
  m_private(new KAabbBoundingVolumePrivate)
{
  P(KAabbBoundingVolumePrivate);
  p.maxMin.max = a.maxExtent() + offset;
  p.maxMin.min = a.minExtent() + offset;
}

KAabbBoundingVolume::~KAabbBoundingVolume()
{
  delete m_private;
}

void KAabbBoundingVolume::operator=(const KAabbBoundingVolume &rhs)
{
  m_private->maxMin = rhs.m_private->maxMin;
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

void KAabbBoundingVolume::shiftCenter(const KVector3D &tr)
{
  P(KAabbBoundingVolumePrivate);
  p.maxMin.max += tr;
  p.maxMin.min += tr;
}

void KAabbBoundingVolume::encompassPoint(const KVector3D &vector)
{
  P(KAabbBoundingVolumePrivate);
  if (p.maxMin.min.x() > vector.x()) p.maxMin.min.setX(vector.x());
  if (p.maxMin.min.y() > vector.y()) p.maxMin.min.setY(vector.y());
  if (p.maxMin.min.z() > vector.z()) p.maxMin.min.setZ(vector.z());
  if (p.maxMin.max.x() < vector.x()) p.maxMin.max.setX(vector.x());
  if (p.maxMin.max.y() < vector.y()) p.maxMin.max.setY(vector.y());
  if (p.maxMin.max.z() < vector.z()) p.maxMin.max.setZ(vector.z());
}

void KAabbBoundingVolume::setMinMaxBounds(const Karma::MinMaxKVector3D &minMax)
{
  P(KAabbBoundingVolumePrivate);
  p.maxMin = minMax;
}

void KAabbBoundingVolume::draw(KTransform3D &t, KColor const &color) const
{
  KAabbBoundingVolume aabb = (*this) * t.toMatrix();
  OpenGLDebugDraw::World::drawAabb(aabb.m_private->maxMin.min, aabb.m_private->maxMin.max, color);
}

void KAabbBoundingVolume::makeCube()
{
  P(KAabbBoundingVolumePrivate);
  KVector3D centroid = center();
  KVector3D halfExtents = (p.maxMin.max - p.maxMin.min) / 2.0f;
  float maxHalfExtent = halfExtents.x();
  if (halfExtents.y() > maxHalfExtent) maxHalfExtent = halfExtents.y();
  if (halfExtents.z() > maxHalfExtent) maxHalfExtent = halfExtents.z();
  p.maxMin.max = centroid + KVector3D(maxHalfExtent, maxHalfExtent, maxHalfExtent);
  p.maxMin.min = centroid - KVector3D(maxHalfExtent, maxHalfExtent, maxHalfExtent);
}

bool KAabbBoundingVolume::contains(const KVector3D &pos) const
{
  P(const KAabbBoundingVolumePrivate);
  if (pos.x() > p.maxMin.max.x()) return false;
  if (pos.y() > p.maxMin.max.y()) return false;
  if (pos.z() > p.maxMin.max.z()) return false;
  if (pos.x() < p.maxMin.min.x()) return false;
  if (pos.y() < p.maxMin.min.y()) return false;
  if (pos.z() < p.maxMin.min.z()) return false;
  return true;
}

KAabbBoundingVolume KAabbBoundingVolume::copyOffset(float x, float y, float z)
{
  return KAabbBoundingVolume(*this, KVector3D(x, y, z));
}

void KAabbBoundingVolume::scale(float k)
{
  P(KAabbBoundingVolumePrivate);
  KVector3D centroid = center();
  p.maxMin.max = (p.maxMin.max - centroid) * k + centroid;
  p.maxMin.min = (p.maxMin.min - centroid) * k + centroid;
}

KVector3D KAabbBoundingVolume::point(int idx) const
{
  P(KAabbBoundingVolumePrivate);
  switch (idx)
  {
  case 0:
    return KVector3D(p.maxMin.min.x(), p.maxMin.min.y(), p.maxMin.min.z());
  case 1:
    return KVector3D(p.maxMin.min.x(), p.maxMin.min.y(), p.maxMin.max.z());
  case 2:
    return KVector3D(p.maxMin.min.x(), p.maxMin.max.y(), p.maxMin.min.z());
  case 3:
    return KVector3D(p.maxMin.max.x(), p.maxMin.min.y(), p.maxMin.min.z());
  case 4:
    return KVector3D(p.maxMin.max.x(), p.maxMin.max.y(), p.maxMin.max.z());
  case 5:
    return KVector3D(p.maxMin.max.x(), p.maxMin.max.y(), p.maxMin.min.z());
  case 6:
    return KVector3D(p.maxMin.max.x(), p.maxMin.min.y(), p.maxMin.max.z());
  case 7:
    return KVector3D(p.maxMin.min.x(), p.maxMin.max.y(), p.maxMin.max.z());
  default:
    qFatal("Invalid index into KAabbBoundingVollume::point(int idx)!");
    break;
  }
  return KVector3D();
}

KAabbBoundingVolume KAabbBoundingVolume::operator*(const KMatrix4x4 &mtx) const
{
  P(const KAabbBoundingVolumePrivate);
  KAabbBoundingVolume retVal;

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
  retVal.m_private->maxMin = Karma::findMinMaxBounds(tVec.begin(), tVec.end());
  return retVal;
}

void KAabbBoundingVolume::constructPrivate()
{
  m_private = new KAabbBoundingVolumePrivate;
}
