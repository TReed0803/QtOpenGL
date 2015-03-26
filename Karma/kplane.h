#ifndef KPLANE_H
#define KPLANE_H KPlane

#include <KVector3D>

class KPlane
{
public:
  KPlane();
  KPlane(KVector3D const &position, KVector3D const &normal);
  KPlane(KVector3D const &a, KVector3D const &b, KVector3D const &c);

  float dot(KVector3D const &point) const;
  bool pointInFront(KVector3D const &point) const;
  bool pointInBack(KVector3D const &point) const;

  static KPlane planeFromPolygon();

private:
  KVector3D m_origin;
  KVector3D m_normal;
};

inline KPlane::KPlane()
{
  // Intentionally Empty
}

inline KPlane::KPlane(KVector3D const &position, KVector3D const &normal) :
  m_origin(position),
  m_normal(normal.normalized())
{
  // Intentionally Empty
}

inline KPlane::KPlane(KVector3D const &a, KVector3D const &b, KVector3D const &c) :
  m_origin((a + b + c) / 3.0f), m_normal(KVector3D::crossProduct(b-a, c-a).normalized())
{
  // Intentionally Empty
}

inline float KPlane::dot(KVector3D const &point) const
{
  return KVector3D::dotProduct(m_normal, point - m_origin);
}

inline bool KPlane::pointInFront(const KVector3D &point) const
{
  return (dot(point) > 0.0f);
}

inline bool KPlane::pointInBack(const KVector3D &point) const
{
  return (dot(point) < 0.0f);
}

#endif // KPLANE_H
