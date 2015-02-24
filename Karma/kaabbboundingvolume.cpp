#include "kaabbboundingvolume.h"
#include <KMacros>
#include <KHalfEdgeMesh>
#include <OpenGLDebugDraw>
#include <KTransform3D>

class KAabbBoundingVolumePrivate
{
public:
  void calculateMinMaxMethod(KHalfEdgeMesh const &mesh);
  KVector3D minimum;
  KVector3D maximum;
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
  KVector3D min = p.minimum * t.toMatrix();
  KVector3D max = p.maximum * t.toMatrix();
  OpenGLDebugDraw::World::drawAabb(min, max, color);
}
