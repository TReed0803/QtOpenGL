#include "ksphereboundingvolume.h"
#include <KMacros>
#include <KHalfEdgeMesh>
#include <KTransform3D>
#include <OpenGLDebugDraw>
#include <KMatrix3x3>
#include <KMath>

class KSphereBoundingVolumePrivate
{
public:
  void calculateCentroidMethod(const KHalfEdgeMesh &mesh);
  void calculateRittersMethod(const KHalfEdgeMesh &mesh);
  void calculateLarssonsMethod(const KHalfEdgeMesh &mesh);
  void calculatePcaMethod(const KHalfEdgeMesh &mesh);
  KVector3D centroid;
  float radius;
private:
  void mostSeparatedPoints(KVector3D *min, KVector3D *max, const KHalfEdgeMesh &mesh, size_t sample);
  void calculateFromDistantPoints(const KHalfEdgeMesh &mesh, size_t sample);
  void expandToContainPoint(const KVector3D &v);
  void calculateFromCovarianceMatrix(const KHalfEdgeMesh &mesh, int iterations);
};

void KSphereBoundingVolumePrivate::calculateCentroidMethod(const KHalfEdgeMesh &mesh)
{
  float tempRadius;
  radius = 0.0f;
  centroid = KVector3D(0.0f, 0.0f, 0.0f);
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();
  for (KHalfEdgeMesh::Vertex const &v : vertices)
  {
    centroid += (v.position - centroid);
  }
  centroid /= vertices.size();
  for (int i = 0; i < vertices.size(); ++i)
  {
    tempRadius = (centroid - vertices[i].position).lengthSquared();
    if (tempRadius > radius) radius = tempRadius;
  }
  radius = std::sqrt(radius);
}

void KSphereBoundingVolumePrivate::calculateRittersMethod(const KHalfEdgeMesh &mesh)
{
  calculateFromDistantPoints(mesh, 6);
  for (KHalfEdgeMesh::Vertex const & v : mesh.vertices())
  {
    expandToContainPoint(v.position);
  }
}

void KSphereBoundingVolumePrivate::calculateLarssonsMethod(const KHalfEdgeMesh &mesh)
{

}

void KSphereBoundingVolumePrivate::calculatePcaMethod(const KHalfEdgeMesh &mesh)
{
  calculateFromCovarianceMatrix(mesh, 6);
  for (KHalfEdgeMesh::Vertex const & v : mesh.vertices())
  {
    expandToContainPoint(v.position);
  }
}

void KSphereBoundingVolumePrivate::mostSeparatedPoints(KVector3D *minimum, KVector3D *maximum, const KHalfEdgeMesh &mesh, size_t sample)
{
  size_t step = mesh.vertices().size() / sample;

  size_t minx = 0, miny = 0, minz = 0, maxx = 0, maxy = 0, maxz = 0;
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();
  for (size_t i = step; i < mesh.vertices().size(); i += step)
  {
    if (vertices[i].position.x() < vertices[minx].position.x()) minx = i;
    if (vertices[i].position.y() < vertices[miny].position.y()) miny = i;
    if (vertices[i].position.z() < vertices[minz].position.z()) minz = i;
    if (vertices[i].position.x() > vertices[maxx].position.x()) maxx = i;
    if (vertices[i].position.y() > vertices[maxy].position.y()) maxy = i;
    if (vertices[i].position.z() > vertices[maxz].position.z()) maxz = i;
  }

  float dist2x = (vertices[maxx].position - vertices[minx].position).lengthSquared();
  float dist2y = (vertices[maxy].position - vertices[miny].position).lengthSquared();
  float dist2z = (vertices[maxz].position - vertices[minz].position).lengthSquared();

  (*maximum) = vertices[maxx].position;
  (*minimum) = vertices[minx].position;
  if (dist2y > dist2x && dist2y > dist2z)
  {
    (*maximum) = vertices[maxy].position;
    (*minimum) = vertices[miny].position;
  }
  if (dist2z > dist2x && dist2z > dist2y)
  {
    (*maximum) = vertices[maxz].position;
    (*minimum) = vertices[minz].position;
  }
}

void KSphereBoundingVolumePrivate::calculateFromDistantPoints(const KHalfEdgeMesh &mesh, size_t sample)
{
  KVector3D min, max;
  mostSeparatedPoints(&min, &max, mesh, sample);

  centroid = (min + max) / 2.0f;
  radius = (max - centroid).length();
}

void KSphereBoundingVolumePrivate::expandToContainPoint(const KVector3D &v)
{
  KVector3D delta = v - centroid;
  float dist2 = delta.lengthSquared();
  if (dist2 > radius * radius)
  {
    float dist = std::sqrt(dist2);
    float newRadius = (radius + dist) / 2.0f;
    float radiusScalar = (newRadius - radius) / dist;
    radius = newRadius;
    centroid += delta * radiusScalar;
  }
}

void KSphereBoundingVolumePrivate::calculateFromCovarianceMatrix(const KHalfEdgeMesh &mesh, int iterations)
{
  KVector3D minimum, maximum;
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();
  KMatrix3x3 covariance = Karma::covarianceMatrix(vertices.begin(), vertices.end(), KHalfEdgeMesh::VertexPositionPred());
  KMatrix3x3 eigenVectors = Karma::jacobi(covariance, iterations);
  KVector3D axis = Karma::maxEigenExtents(eigenVectors);
  Karma::maxSeperatedAlongAxis(vertices.begin(), vertices.end(), KHalfEdgeMesh::VertexPositionPred(), axis, &minimum, &maximum);
  float dist = (maximum - minimum).length();
  radius = dist / 2.0f;
  centroid = (maximum + minimum) / 2.0f;
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
  KVector3D const &scale = t.scale();
  float scalar = scale.x();
  if (scale.y() > scalar) scalar = scale.y();
  if (scale.z() > scalar) scalar = scale.z();
  OpenGLDebugDraw::World::drawSphere(position, p.radius * scalar, color);
}
