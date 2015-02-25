#include "ksphereboundingvolume.h"
#include <KMacros>
#include <KHalfEdgeMesh>
#include <KTransform3D>
#include <OpenGLDebugDraw>

class KSphereBoundingVolumePrivate
{
public:
  void calculateCentroidMethod(const KHalfEdgeMesh &mesh);
  void calculateRittersMethod(const KHalfEdgeMesh &mesh);
  void calculateLarssonsMethod(const KHalfEdgeMesh &mesh);
  void calculatePcaMethod(const KHalfEdgeMesh &mesh);
  KVector3D centroid;
  float radius;
};

void KSphereBoundingVolumePrivate::calculateCentroidMethod(const KHalfEdgeMesh &mesh)
{
  float tempRadius;
  radius = 0.0f;
  centroid = KVector3D(0.0f, 0.0f, 0.0f);
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();
  for (int i = 0; i < vertices.size(); ++i)
  {
    centroid += (vertices[i].position - centroid) / (i + 1.0f);
  }
  for (int i = 0; i < vertices.size(); ++i)
  {
    tempRadius = (centroid - vertices[i].position).lengthSquared();
    if (tempRadius > radius) radius = tempRadius;
  }
}

void KSphereBoundingVolumePrivate::calculateRittersMethod(const KHalfEdgeMesh &mesh)
{

}

void KSphereBoundingVolumePrivate::calculateLarssonsMethod(const KHalfEdgeMesh &mesh)
{

}

void KSphereBoundingVolumePrivate::calculatePcaMethod(const KHalfEdgeMesh &mesh)
{

}

KSphereBoundingVolume::KSphereBoundingVolume(const KHalfEdgeMesh &mesh, Method method) :
  m_private(new KSphereBoundingVolumePrivate)
{
  P(KSphereBoundingVolumePrivate);
  switch (method)
  {
  case CentroidMethod:
    p.calculateCentroidMethod(mesh);
    break;
  case RittersMethod:
    p.calculateRittersMethod(mesh);
    break;
  case LarssonsMethod:
    p.calculateLarssonsMethod(mesh);
    break;
  case PcaMethod:
    p.calculatePcaMethod(mesh);
    break;
  }
}

KSphereBoundingVolume::~KSphereBoundingVolume()
{
  delete m_private;
}

void KSphereBoundingVolume::draw(KTransform3D &t, const KColor &color)
{
  P(KSphereBoundingVolumePrivate);
  KVector3D position = p.centroid * t.toMatrix();
  OpenGLDebugDraw::World::drawSphere(position, p.radius, color);
}
