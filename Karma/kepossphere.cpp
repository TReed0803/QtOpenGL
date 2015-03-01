#include "kepossphere.h"

struct Sphere
{
  Sphere(Sphere const &a, Sphere const &b);
  Sphere(KVector3D const &a, KVector3D const &b);
  float radius;
  KVector3D origin;
};

Sphere::Sphere(const Sphere &a, const Sphere &b)
{
  // Distance to A
  KVector3D aNormal = (a.origin - b.origin);
  float dist = aNormal.length();
  aNormal /= dist;

  // Find two extrenal points
  KVector3D pointA = a.origin + aNormal * a.radius;
  KVector3D pointB = b.origin - aNormal * b.radius;

  // Solve for sphere
  origin = (pointA + pointB) / 2.0f;
  radius = (pointA - pointB).length() / 2.0f;
}

Sphere::Sphere(const KVector3D &a, const KVector3D &b)
{
  origin = (a + b) / 2.0f;
  radius = (a - b).length() / 2.0f;
}

Sphere WelzlSphere(KMinMaxVectorCloud::const_iterator begin, KMinMaxVectorCloud::const_iterator end)
{
  size_t numPoints = std::distance(begin, end);
  if (numPoints == 1)
  {
    return Sphere(begin[0].max, begin[0].min);
  }

  KMinMaxVectorCloud::const_iterator halfWay = begin + numPoints / 2;
  return Sphere(WelzlSphere(begin, halfWay), WelzlSphere(halfWay, end));
}

void KEposSphere::calculateMinimumSphere(const_iterator begin, const_iterator end)
{
  Sphere s = WelzlSphere(begin, end);
  centroid = s.origin;
  radius = s.radius;
}
