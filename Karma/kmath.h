#ifndef KMATH_H
#define KMATH_H KMath

#include <KMatrix3x3>

namespace Karma
{
  static const float Pi;
  static const float PiHalf;
  static const float TwoPi;

  template <typename It, typename Func>
  KMatrix3x3 covarianceMatrix(It begin, It end, Func f);
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
    e00 += vCentered.x() * vCentered.x();
    e00 += vCentered.x() * vCentered.y();
    e00 += vCentered.x() * vCentered.z();
    e00 += vCentered.y() * vCentered.z();
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

#endif // KMATH_H
