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
  KVector3D minimum;
  KVector3D maximum;
  KVector3D centroid;
  KVector3D halfLengths;
};

void KAabbBoundingVolumePrivate::calculateMinMaxMethod(const KHalfEdgeMesh &mesh)
{
  minimum = KVector3D( std::numeric_limits<float>::infinity(),  std::numeric_limits<float>::infinity(),  std::numeric_limits<float>::infinity());
  maximum = KVector3D(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
  for (KHalfEdgeMesh::Vertex const &v : mesh.vertices())
  {
    if (minimum.x() > v.position.x()) minimum.setX(v.position.x());
    if (minimum.y() > v.position.y()) minimum.setY(v.position.y());
    if (minimum.z() > v.position.z()) minimum.setZ(v.position.z());
    if (maximum.x() < v.position.x()) maximum.setX(v.position.x());
    if (maximum.y() < v.position.y()) maximum.setY(v.position.y());
    if (maximum.z() < v.position.z()) maximum.setZ(v.position.z());
  }

  centroid = (maximum + minimum) / 2.0f;
  halfLengths = (maximum - minimum) / 2.0f;
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

  // Find the Aabb formed by the rotated Aabb
  KVector3D minimum( std::numeric_limits<float>::infinity(),  std::numeric_limits<float>::infinity(),  std::numeric_limits<float>::infinity());
  KVector3D maximum(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

  KVector3D verts[] =
  {
    p.minimum, p.maximum,
    KVector3D( p.minimum.x(), p.maximum.y(), p.maximum.z()),
    KVector3D( p.minimum.x(), p.minimum.y(), p.maximum.z()),
    KVector3D( p.maximum.x(), p.minimum.y(), p.maximum.z()),
    KVector3D( p.maximum.x(), p.minimum.y(), p.minimum.z()),
    KVector3D( p.maximum.x(), p.maximum.y(), p.minimum.z()),
    KVector3D( p.minimum.x(), p.maximum.y(), p.minimum.z())
  };

  for (int i = 0; i < sizeof(verts) / sizeof(verts[0]); ++i)
  {
    KVector3D vertex = t.toMatrix() * verts[i];
    if (minimum.x() > vertex.x()) minimum.setX(vertex.x());
    if (minimum.y() > vertex.y()) minimum.setY(vertex.y());
    if (minimum.z() > vertex.z()) minimum.setZ(vertex.z());
    if (maximum.x() < vertex.x()) maximum.setX(vertex.x());
    if (maximum.y() < vertex.y()) maximum.setY(vertex.y());
    if (maximum.z() < vertex.z()) maximum.setZ(vertex.z());
  }

  OpenGLDebugDraw::World::drawAabb(minimum, maximum, color);
}
