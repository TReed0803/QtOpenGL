#include "korientedboundingvolume.h"
#include <KMacros>
#include <KVector3D>
#include <KMath>
#include <KHalfEdgeMesh>
#include <KTransform3D>
#include <OpenGLDebugDraw>
#include <KColor>

class KOrientedBoundingVolumePrivate
{
public:
  void calculatePcaMethod(KHalfEdgeMesh const &mesh);
  KVector3D centroid;
  KMatrix3x3 axes;
  KVector3D extents;
  KVector3D minimum[3];
  KVector3D maximum[3];
};

void KOrientedBoundingVolumePrivate::calculatePcaMethod(const KHalfEdgeMesh &mesh)
{
  KVector3D tempAxes[3];
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();
  KMatrix3x3 covariance = Karma::covarianceMatrix(vertices.begin(), vertices.end(), KHalfEdgeMesh::VertexPositionPred());
  axes = Karma::jacobi(covariance, 50);
  Karma::extractEigenVectors(axes, tempAxes);
  Karma::lengthsAlongAxes<decltype(vertices.begin()),KHalfEdgeMesh::VertexPositionPred,3>(vertices.begin(), vertices.end(), KHalfEdgeMesh::VertexPositionPred(), tempAxes, reinterpret_cast<float*>(&extents), minimum, maximum, &centroid);
  extents /= 2.0f; // Convert to half-extents.
}

KOrientedBoundingVolume::KOrientedBoundingVolume(const KHalfEdgeMesh &mesh, Method method) :
  m_private(new KOrientedBoundingVolumePrivate)
{
  P(KOrientedBoundingVolumePrivate);
  switch (method)
  {
  case PcaMethod:
    p.calculatePcaMethod(mesh);
    break;
  }
}

KOrientedBoundingVolume::~KOrientedBoundingVolume()
{
  delete m_private;
}

void KOrientedBoundingVolume::draw(KTransform3D &t, const KColor &color)
{
  P(KOrientedBoundingVolumePrivate);
  KVector3D center = p.centroid * t.toMatrix();
  KMatrix3x3 eigenVecs = KMatrix3x3(t.toMatrix()) * p.axes;
  OpenGLDebugDraw::World::drawObb(center, eigenVecs, p.extents, color);

  for (int i = 0; i < 3; ++i)
  {
    OpenGLDebugDraw::World::drawPoint(p.maximum[i] * t.toMatrix(), Qt::blue);
    OpenGLDebugDraw::World::drawPoint(p.minimum[i] * t.toMatrix(), Qt::blue);
  }
}



