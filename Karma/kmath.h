#ifndef KMATH_H
#define KMATH_H KMath

#include <KVector3D>
#include <KMatrix3x3>
#include <limits>

namespace Karma
{
  static const float Pi;
  static const float PiHalf;
  static const float TwoPi;

  template <typename It, typename Func>
  KMatrix3x3 covarianceMatrix(It begin, It end, Func f);
  void symSchur2(KMatrix3x3 const &symMtx, int p, int q, float *cosine, float *sine);
  KMatrix3x3 jacobi(KMatrix3x3 covar, int iterations);
  KVector3D maxEigenExtents(KMatrix3x3 const &eigenVecs);
  template <typename It, typename Func>
  void maxSeperatedAlongAxis(It begin, It end, Func f, KVector3D axis, KVector3D *min, KVector3D *max);
  template <typename It, typename Func>
  void lengthsAlongAxes(It begin, It end, Func f, KVector3D axes[3], float dist[3], KVector3D *centroid);
  void extractEigenVectors(KMatrix3x3 const &eigenVecs, KVector3D axes[3]);
  template <typename It, typename Func, size_t N>
  void lengthsAlongAxes(It begin, It end, Func f, KVector3D axes[N], float dist[N], KVector3D minimum[N], KVector3D maximum[N], KVector3D *centroid)
  {
    int i;
    float sDist;
    float maxProjDist[N];
    float minProjDist[N];
    for (size_t i = 0; i < N; ++i)
    {
      maxProjDist[i] = -std::numeric_limits<float>::infinity();
      minProjDist[i] =  std::numeric_limits<float>::infinity();
    }

    while (begin != end)
    {
      for (i = 0; i < N; ++i)
      {
        sDist = KVector3D::dotProduct(f(*begin), axes[i]);
        if (sDist > maxProjDist[i])
        {
          maxProjDist[i] = sDist;
          maximum[i] = sDist * axes[i];
        }
        if (sDist < minProjDist[i])
        {
          minProjDist[i] = sDist;
          minimum[i] = sDist * axes[i];
        }
      }
      ++begin;
    }

    (*centroid) = KVector3D(0.0f, 0.0f, 0.0f);
    for (i = 0; i < 3; ++i)
    {
      dist[i] = (maximum[i] - minimum[i]).length();
      (*centroid) += (maximum[i] + minimum[i]) / 2.0f;
    }
  }
}

template <typename It, typename Func>
KMatrix3x3 Karma::covarianceMatrix(It begin, It end, Func f)
{
  It origBegin = begin;
  int count = std::distance(begin, end);
  float k = 1.0f / float(count);
  KVector3D center;
  float e00, e11, e22, e01, e02, e12;

  // Calculate average center
  while (begin != end)
  {
    center += f(*begin);
    ++begin;
  }
  center /= float(count);

  // Calculate covariance matrix
  begin = origBegin;
  KVector3D vCentered;
  e00 = e11 = e22 = e01 = e02 = e12 = 0.0f;
  while (begin != end)
  {
    vCentered = f(*begin) - center;
    e00 += vCentered.x() * vCentered.x();
    e11 += vCentered.y() * vCentered.y();
    e22 += vCentered.z() * vCentered.z();
    e01 += vCentered.x() * vCentered.y();
    e02 += vCentered.x() * vCentered.z();
    e12 += vCentered.y() * vCentered.z();
    ++begin;
  }

  // Apply scale to whole matrix
  KMatrix3x3 covariance;
  covariance[0][0] = k * e00;
  covariance[1][1] = k * e11;
  covariance[2][2] = k * e22;
  covariance[0][1] = covariance[1][0] = k * e01;
  covariance[0][2] = covariance[2][0] = k * e02;
  covariance[1][2] = covariance[2][1] = k * e12;

  return covariance;
}

template <typename It, typename Func>
void Karma::maxSeperatedAlongAxis(It begin, It end, Func f, KVector3D axis, KVector3D *min, KVector3D *max)
{
  float sDist;
  float maxProjDist = -std::numeric_limits<float>::infinity();
  float minProjDist =  std::numeric_limits<float>::infinity();
  while (begin != end)
  {
    sDist = KVector3D::dotProduct(f(*begin), axis);
    if (sDist > maxProjDist)
    {
      maxProjDist = sDist;
      (*max) = f(*begin);
    }
    if (sDist < minProjDist)
    {
      minProjDist = sDist;
      (*min) = f(*begin);
    }
    ++begin;
  }
}

template <typename It, typename Func>
void Karma::lengthsAlongAxes(It begin, It end, Func f, KVector3D axes[3], float dist[3], KVector3D *centroid)
{
  int i;
  KVector3D max[3];
  KVector3D min[3];
  float sDist;

  float maxProjDist[3] =
  {
    -std::numeric_limits<float>::infinity(),
    -std::numeric_limits<float>::infinity(),
    -std::numeric_limits<float>::infinity()
  };
  float minProjDist[3] =
  {
    std::numeric_limits<float>::infinity(),
    std::numeric_limits<float>::infinity(),
    std::numeric_limits<float>::infinity()
  };

  while (begin != end)
  {
    for (i = 0; i < 3; ++i)
    {
      sDist = KVector3D::dotProduct(f(*begin), axes[i]);
      if (sDist > maxProjDist[i])
      {
        maxProjDist[i] = sDist;
        max[i] = f(*begin);
      }
      if (sDist < minProjDist[i])
      {
        minProjDist[i] = sDist;
        min[i] = f(*begin);
      }
    }
    ++begin;
  }

  (*centroid) = KVector3D(0.0f, 0.0f, 0.0f);
  for (i = 0; i < 3; ++i)
  {
    dist[i] = (max[i] - min[i]).length();
    (*centroid) += max[i] + min[i];
  }
  (*centroid) /= 6.0f;
}

#endif // KMATH_H
