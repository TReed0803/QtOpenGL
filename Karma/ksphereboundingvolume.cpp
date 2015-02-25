#include "ksphereboundingvolume.h"
#include <KMacros>
#include <KHalfEdgeMesh>
#include <KTransform3D>
#include <OpenGLDebugDraw>
#include <KMatrix3x3>

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
  void mostSeparatedPointsAlongVector(KVector3D *min, KVector3D *max, const KHalfEdgeMesh &mesh, KVector3D const &axis);
  void calculateFromDistantPoints(const KHalfEdgeMesh &mesh, size_t sample);
  void expandToContainPoint(const KVector3D &v);
  void covarianceMatrix(const KHalfEdgeMesh &mesh, KMatrix3x3 *mtx);
  void calculateFromCovarianceMatrix(const KHalfEdgeMesh &mesh, int iterations);
  void jacobiDecomposition(KMatrix3x3 &covar, KMatrix3x3 *eigen, int iterations);
  void SymSchur2(KMatrix3x3 const &covar, int p, int q, float *c, float *s);
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

void KSphereBoundingVolumePrivate::mostSeparatedPointsAlongVector(KVector3D *min, KVector3D *max, const KHalfEdgeMesh &mesh, const KVector3D &axis)
{
  KVector3D projPoint;
  float sDist;
  float maxProjDist = -std::numeric_limits<float>::infinity();
  float minProjDist =  std::numeric_limits<float>::infinity();
  for (KHalfEdgeMesh::Vertex const &v : mesh.vertices())
  {
    sDist = KVector3D::dotProduct(v.position, axis);
    if (sDist > maxProjDist)
    {
      maxProjDist = sDist;
      (*max) = v.position;
    }
    if (sDist < minProjDist)
    {
      minProjDist = sDist;
      (*min) = v.position;
    }
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

void KSphereBoundingVolumePrivate::covarianceMatrix(const KHalfEdgeMesh &mesh, KMatrix3x3 *mtx)
{
  float k = 1.0f / float(mesh.vertices().size());
  KVector3D covarianceTerm;
  float e00, e11, e22, e01, e02, e12;

  // Calculate average center
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();
  for (KHalfEdgeMesh::Vertex const &v : vertices)
  {
    centroid += (v.position - centroid);
  }
  centroid /= vertices.size();

  // Calculate covariance matrix
  KVector3D vCentered;
  e00 = e11 = e22 = e01 = e02 = e12 = 0.0f;
  for (KHalfEdgeMesh::Vertex const &v : vertices)
  {
    vCentered = v.position - centroid;
    e00 += v.position.x() * v.position.x();
    e11 += v.position.y() * v.position.y();
    e22 += v.position.z() * v.position.z();
    e00 += v.position.x() * v.position.x();
    e00 += v.position.x() * v.position.y();
    e00 += v.position.x() * v.position.z();
    e00 += v.position.y() * v.position.z();
  }

  (*mtx)[0][0] = k * e00;
  (*mtx)[1][1] = k * e11;
  (*mtx)[2][2] = k * e22;
  (*mtx)[0][1] = (*mtx)[1][0] = k * e01;
  (*mtx)[0][2] = (*mtx)[2][0] = k * e02;
  (*mtx)[1][2] = (*mtx)[2][1] = k * e12;
}

void KSphereBoundingVolumePrivate::calculateFromCovarianceMatrix(const KHalfEdgeMesh &mesh, int iterations)
{
  KMatrix3x3 covariance, eigenVectors;

  covarianceMatrix(mesh, &covariance);
  jacobiDecomposition(covariance, &eigenVectors, iterations);

  // Find longest magnitude eigenvalues
  KVector3D eigen;
  int maxc = 0;
  float maxf, maxe = std::abs(eigenVectors[0][0]);
  maxf = std::abs(eigenVectors[1][1]);
  if (maxf > maxe) maxc = 1, maxe = maxf;
  maxf = std::abs(eigenVectors[2][2]);
  if (maxf > maxe) maxc = 2, maxe = maxf;
  eigen.setX(eigenVectors[0][maxc]);
  eigen.setY(eigenVectors[1][maxc]);
  eigen.setZ(eigenVectors[2][maxc]);

  // Find most extreme points along 'e'
  KVector3D minimum, maximum;
  mostSeparatedPointsAlongVector(&minimum, &maximum, mesh, eigen);
  float dist = (maximum - minimum).length();
  radius = dist / 2.0f;
  centroid = (maximum + minimum) / 2.0f;
}

void KSphereBoundingVolumePrivate::jacobiDecomposition(KMatrix3x3 &covar, KMatrix3x3 *eigen, int iterations)
{
  int p, q;
  float prevOff, c, s;
  KMatrix3x3 jacobiMtx;
  eigen->setToIdentity();

  for (int n = 0; n < iterations; ++n)
  {

    // Largest off-diagonal element
    p = 0; q = 1;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        if (i == j) continue;
        if (std::abs(covar[i][j]) > std::abs(covar[p][q]))
        {
          p = i;
          q = j;
        }
      }
    }

    // Compute the jacobi rotation
    SymSchur2(covar, p, q, &c, &s);
    jacobiMtx.setToIdentity();
    jacobiMtx[p][p] = jacobiMtx[q][q] = c;
    jacobiMtx[p][q] = s; jacobiMtx[q][p] = -s;

    // Cumulate rotations
    (*eigen) *= jacobiMtx;
    covar = (jacobiMtx.transposed() * covar) * jacobiMtx;
    float off = 0.0f;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        if (i == j) continue;
        off += covar[i][j] * covar[i][j];
      }
    }

    // Stop when no longer decreasing
    if (n > 2 && off >= prevOff)
      return;

    prevOff = off;
  }
}

void KSphereBoundingVolumePrivate::SymSchur2(const KMatrix3x3 &covar, int p, int q, float *c, float *s)
{
  if (std::abs(covar[p][q] > 0.0001f))
  {
    float r = (covar[q][q] - covar[p][p]) / (2.0f * covar[p][q]);
    float t;
    if (r >= 0.0f)
    {
      t = 1.0f / (r + std::sqrt(1.0f + r * r));
    }
    else
    {
      t = 1.0f / (r - std::sqrt(1.0f + r * r));
    }
    (*c) = 1.0f / std::sqrt(1.0f + t * t);
    (*s) = t * (*c);
  }
  else
  {
    (*c) = 1.0f;
    (*s) = 0.0f;
  }
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
