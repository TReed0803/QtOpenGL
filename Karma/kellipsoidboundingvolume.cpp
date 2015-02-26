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
  KVector3D tempAxes[3];
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();
  KMatrix3x3 covariance = Karma::covarianceMatrix(vertices.begin(), vertices.end(), KHalfEdgeMesh::VertexPositionPred());
  axes = Karma::jacobi(covariance, 50);
  Karma::extractColumnVectors(axes, tempAxes);
  centroid = Karma::calculateCentroid<3>(vertices.begin(), vertices.end(), KHalfEdgeMesh::VertexPositionPred(), tempAxes, reinterpret_cast<float*>(&extents));
  extents /= 2.0f; // Convert to half-extents.
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

  KVector3D extractedAxes[3];
  KVector3D center = t.toMatrix() * p.centroid;
  KMatrix3x3 scaledAxes = KMatrix3x3(t.toMatrix()) * p.axes;
  Karma::extractColumnVectors(scaledAxes, extractedAxes);
  extractedAxes[0] *= p.extents.x();
  extractedAxes[1] *= p.extents.y();
  extractedAxes[2] *= p.extents.z();
  KVector3D scaledExtents(extractedAxes[0].length(), extractedAxes[1].length(), extractedAxes[2].length());
  extractedAxes[0] /= scaledExtents.x();
  extractedAxes[1] /= scaledExtents.y();
  extractedAxes[2] /= scaledExtents.z();

  // x^2   y^2   z^2     //
  // --- + --- + --- = 1 //
  //  a     b     c      //

  // We are going to only measure radii about the x-axis.
  // So the radius at any given step along x,y,z is:
  //       sqrt(b) * sqrt(a - x^2)   //
  // y = --------------------------- //
  //              sqrt(a)            //

  float sqrtA = std::sqrt(scaledExtents.x());
  float sqrtB = std::sqrt(scaledExtents.y());
  float denom = sqrtB / sqrtA;
  float deltaX = scaledExtents.x() / float(12);
  float y;

  // 5 is just some subvision value
  OpenGLDebugDraw::World::drawCircle(p.centroid, extractedAxes[0], sqrtB, color);
  for (float x = deltaX; x < scaledExtents.x(); x += deltaX)
  {
    y = std::sqrt(scaledExtents.x() - x * x) * denom;
    OpenGLDebugDraw::World::drawCircle(center + extractedAxes[0] * x, extractedAxes[0], y, color);
    OpenGLDebugDraw::World::drawCircle(center - extractedAxes[0] * x, extractedAxes[0], y, color);
  }

  OpenGLDebugDraw::World::drawObb(center, scaledAxes, p.extents, color);
}
