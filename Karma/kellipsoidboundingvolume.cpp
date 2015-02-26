#include "kellipsoidboundingvolume.h"
#include <KMacros>
#include <KVector3D>
#include <KMath>
#include <KHalfEdgeMesh>
#include <KTransform3D>
#include <OpenGLDebugDraw>

class KEllipsoidBoundingVolumePrivate
{
public:
  void calculatePcaMethod(KHalfEdgeMesh const &mesh);
  KVector3D centroid;
  KMatrix3x3 axes;
  KVector3D extents;
};

void KEllipsoidBoundingVolumePrivate::calculatePcaMethod(const KHalfEdgeMesh &mesh)
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
  KVector3D axisA = extremal[0].max - extremal[0].min;
  KVector3D axisB = extremal[1].max - extremal[1].min;
  KVector3D axisC = extremal[2].max - extremal[2].min;
  extents.setX(axisA.length());
  extents.setY(axisB.length());
  extents.setZ(axisC.length());
  axisA /= extents.x();
  axisB /= extents.y();
  axisC /= extents.z();
  extents /= 2.0f;
  centroid  = (extremal[0].max + extremal[0].min) / 2.0f;
  centroid += (extremal[1].max + extremal[1].min) / 2.0f;
  centroid += (extremal[2].max + extremal[2].min) / 2.0f;
  Karma::reconstructMatrixByColumnVectors(&axes, axisA, axisB, axisC);
}

KEllipsoidBoundingVolume::KEllipsoidBoundingVolume(KHalfEdgeMesh const &mesh, Method method) :
  m_private(new KEllipsoidBoundingVolumePrivate)
{
  P(KEllipsoidBoundingVolumePrivate);
  switch (method)
  {
  case PcaMethod:
    p.calculatePcaMethod(mesh);
    break;
  }
}

KEllipsoidBoundingVolume::~KEllipsoidBoundingVolume()
{
  delete m_private;
}

void KEllipsoidBoundingVolume::draw(KTransform3D &t, const KColor &color)
{
  P(KEllipsoidBoundingVolumePrivate);

  // x^2   y^2   z^2     //
  // --- + --- + --- = 1 //
  //  a     b     c      //

  // We are going to only measure radii about the x-axis.
  // So the radius at any given step along x,y,z is:
  //       sqrt(b) * sqrt(a - x^2)   //
  // y = --------------------------- //
  //              sqrt(a)            //

  KVector3D extractedAxes[3];
  Karma::decomposeMatrixeByColumnVectors(p.axes, extractedAxes);
  KVector3D scaledExtents = p.extents;
  KVector3D center = p.centroid;

  float sqrtA = std::sqrt(scaledExtents.x());
  float sqrtB = std::sqrt(scaledExtents.y());
  float denom = sqrtB / sqrtA;
  float deltaX = scaledExtents.x() / float(12);
  float y;

  // 5 is just some subvision value
  OpenGLDebugDraw::World::drawCircle(center, extractedAxes[0], sqrtB, color);
  for (float x = deltaX; x < scaledExtents.x(); x += deltaX)
  {
    y = std::sqrt(scaledExtents.x() - x * x) * denom;
    OpenGLDebugDraw::World::drawCircle(center + extractedAxes[0] * x, extractedAxes[0], y, color);
    OpenGLDebugDraw::World::drawCircle(center - extractedAxes[0] * x, extractedAxes[0], y, color);
  }

  OpenGLDebugDraw::World::drawObb(center, p.axes, p.extents, color);
}
