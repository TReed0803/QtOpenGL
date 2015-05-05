#include "kellipsoidboundingvolume.h"
#include <KMacros>
#include <KVector3D>
#include <KMath>
#include <KHalfEdgeMesh>
#include <KTransform3D>
#include <OpenGLDebugDraw>
#include <KMatrix3x3>

class KEllipsoidBoundingVolumePrivate
{
public:
  void calculatePcaMethod(KHalfEdgeMesh const &mesh);
  KVector3D centroid;
  KMatrix3x3 axes;
  KVector3D extents;
private:
  void calculateUsingCovarianceMatrix(KHalfEdgeMesh const &mesh, int iterations);
};

void KEllipsoidBoundingVolumePrivate::calculatePcaMethod(const KHalfEdgeMesh &mesh)
{
  calculateUsingCovarianceMatrix(mesh, 50);
  /*
  for (KHalfEdgeMesh::Vertex const &v : mesh.vertices())
  {
    float extentsScalar;
    KVector3D translatedPoint;
    KVector3D rotatedPoint;
    KVector3D scaledPoint;
    translatedPoint = v.position - centroid;
    rotatedPoint = axes * translatedPoint;
    scaledPoint = rotatedPoint / extents;
    extentsScalar = KVector3D::dotProduct(scaledPoint, scaledPoint);
    while (extentsScalar - 1.0f > 0.01f)
    {
      // Find the point intersection between the center->v and ellipse
      KVector3D test = scaledPoint - scaledPoint.normalized();
      KVector3D positiveScale(std::abs(test.x()), std::abs(test.y()), std::abs(test.z()));
      extents += positiveScale * positiveScale;
      scaledPoint = rotatedPoint / extents;
      extentsScalar = KVector3D::dotProduct(scaledPoint, scaledPoint);
    }
  }
  */
}

void KEllipsoidBoundingVolumePrivate::calculateUsingCovarianceMatrix(const KHalfEdgeMesh &mesh, int iterations)
{
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();

  // Solve for the covariance matrix
  KMatrix3x3 covariance =
    Karma::covarianceMatrix(
      vertices.begin(),
      vertices.end(),
      KHalfEdgeMesh::VertexPositionPred()
    );
  axes = Karma::jacobi(covariance, iterations);

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
  extents *= extents;
  centroid  = (extremal[0].max + extremal[0].min) / 2.0f;
  centroid += (extremal[1].max + extremal[1].min) / 2.0f;
  centroid += (extremal[2].max + extremal[2].min) / 2.0f;
  Karma::reconstructMatrixByColumnVectors(&axes, axisA, axisB, axisC);
}

KEllipsoidBoundingVolume::KEllipsoidBoundingVolume() :
  m_private(0)
{
  // Intentionally Empty
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

void KEllipsoidBoundingVolume::draw(KTransform3D &t, const KColor &color) const
{
  P(KEllipsoidBoundingVolumePrivate);

  KVector3D extractedAxes[3];
  KMatrix3x3 rotatedAxes = KMatrix3x3(t.toMatrix()) * p.axes;
  Karma::decomposeMatrixeByColumnVectors(rotatedAxes, extractedAxes);
  extractedAxes[0].normalize();
  extractedAxes[1].normalize();
  extractedAxes[2].normalize();
  KVector3D scaledExtents = p.extents * t.scale() * t.scale();
  KVector3D center = t.toMatrix() * p.centroid;

  float sqrtA = std::sqrt(scaledExtents.x());
  float sqrtB = std::sqrt(scaledExtents.y());
  float sqrtC = std::sqrt(scaledExtents.z());
  float denomY = sqrtB / sqrtA;
  float denomZ = sqrtC / sqrtA;
  float deltaX = 0.3f;
  float sqrtAMinusASquared, y, z;

  // x^2   y^2   z^2     //
  // --- + --- + --- = 1 //
  // a^2   b^2   c^2     //

  // We are going to only measure radii about the x-axis.
  // So the radius at any given step along x,y,z is:
  //       sqrt(b) * sqrt(a - x^2)   //
  // y = --------------------------- //
  //              sqrt(a)            //
/*
  // Create the latitude
  OpenGLDebugDraw::World::drawOval(center, extractedAxes[0], extractedAxes[1], sqrtB, sqrtC, color);
  for (float x = deltaX; x < scaledExtents.x(); x += deltaX)
  {
    sqrtAMinusASquared = std::sqrt(scaledExtents.x() - x * x);
    y = sqrtAMinusASquared * denomY;
    z = sqrtAMinusASquared * denomZ;
    OpenGLDebugDraw::World::drawOval(center + extractedAxes[0] * x, extractedAxes[0], extractedAxes[1], y, z, color);
    OpenGLDebugDraw::World::drawOval(center - extractedAxes[0] * x, extractedAxes[0], extractedAxes[1], y, z, color);
  }

  */
  // We are going to only measure radii about the x-axis.
  // So the radius at any given step along x,y,z is:
  //       sqrt(b) * sqrt(a - x^2)   //
  // y = --------------------------- //
  //              sqrt(a)            //

  // Longitude (segments)
  float angle = 0.0f;
  float segmentAngle = Karma::Pi / 12.0f;
  for (int i = 0; i < 12; ++i)
  {
    float cosine = std::cos(angle);
    float sine = std::sin(angle);
    OpenGLDebugDraw::World::drawOval(center, cosine * extractedAxes[1] + sine * extractedAxes[2], extractedAxes[1], std::sqrt(std::pow(cosine * sqrtC, 2.0) + std::pow(sine * sqrtB, 2.0f)), sqrtA, 12, color);
    angle += segmentAngle;
  }

  // Latitude (rings)
  OpenGLDebugDraw::World::drawOval(center, extractedAxes[0], extractedAxes[1], sqrtB, sqrtC, color);
  for (float x = deltaX; x < scaledExtents.x(); x += deltaX)
  {
    sqrtAMinusASquared = std::sqrt(scaledExtents.x() - x * x);
    y = sqrtAMinusASquared * denomY;
    z = sqrtAMinusASquared * denomZ;
    OpenGLDebugDraw::World::drawOval(center + extractedAxes[0] * x, extractedAxes[0], extractedAxes[1], y, z, color);
    OpenGLDebugDraw::World::drawOval(center - extractedAxes[0] * x, extractedAxes[0], extractedAxes[1], y, z, color);
  }

}
