#include "korientedboundingvolume.h"
#include <KMacros>
#include <KVector3D>
#include <KMath>
#include <KHalfEdgeMesh>
#include <KTransform3D>
#include <OpenGLDebugDraw>

class KOrientedBoundingVolumePrivate
{
public:
  void calculatePcaMethod(KHalfEdgeMesh const &mesh);
  KVector3D centroid;
  KMatrix3x3 axes;
  KVector3D extents;
};

void KOrientedBoundingVolumePrivate::calculatePcaMethod(const KHalfEdgeMesh &mesh)
{
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();

  // Solve for the covariance matrix
  KMatrix3x3 covariance =
    Karma::covarianceMatrix(
      vertices.begin(),
      vertices.end(),
      KHalfEdgeMesh::VertexPositionPred()
    );
  axes = Karma::jacobi(covariance, 50);

  // Find the extremal projected points along each axis
  std::vector<KVector3D> extractedAxes = Karma::decomposeMatrixeByColumnVectors(axes);
  std::vector<Karma::MinMaxKVector3D> extremal =
    Karma::findExtremalProjectedPointsAlongAxes(
      vertices.begin(),
      vertices.end(),
      extractedAxes.begin(),
      extractedAxes.end(),
      KHalfEdgeMesh::VertexPositionPred()
    );

  // Store information for the centroid and extent
  extents.setX((extremal[0].max - extremal[0].min).length() / 2.0f);
  extents.setY((extremal[1].max - extremal[1].min).length() / 2.0f);
  extents.setZ((extremal[2].max - extremal[2].min).length() / 2.0f);
  centroid  = (extremal[0].max + extremal[0].min) / 2.0f;
  centroid += (extremal[1].max + extremal[1].min) / 2.0f;
  centroid += (extremal[2].max + extremal[2].min) / 2.0f;
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

void KOrientedBoundingVolume::draw(KTransform3D &t, const KColor &color) const
{
  P(KOrientedBoundingVolumePrivate);
  KVector3D center = t.toMatrix() * p.centroid;
  KMatrix3x3 eigenVecs = KMatrix3x3(t.toMatrix()) * p.axes;
  OpenGLDebugDraw::World::drawObb(center, eigenVecs, p.extents, color);
}



