#ifndef KPLANE_H
#define KPLANE_H KPlane

#include <KVector3D>

class KPlane
{
public:
  KPlane();
  KPlane(KVector3D const &position, KVector3D const &normal);
  KPlane(KVector3D const &a, KVector3D const &b, KVector3D const &c);

  void set(float a, float b, float c, float d);
  float dot(KVector3D const &point) const;
  bool pointInFront(KVector3D const &point) const;
  bool pointInBack(KVector3D const &point) const;

  static KPlane planeFromPolygon();

private:
  KVector3D m_normal;
  float m_dTerm;
};

inline KPlane::KPlane()
{
  // Intentionally Empty
}

inline KPlane::KPlane(KVector3D const &position, KVector3D const &normal) :
  m_normal(normal.normalized()),
  m_dTerm(-KVector3D::dotProduct(m_normal, position))
{
  // Intentionally Empty
}

inline KPlane::KPlane(KVector3D const &a, KVector3D const &b, KVector3D const &c) :
  m_normal(KVector3D::crossProduct(b-a, c-a).normalized()),
  m_dTerm(-KVector3D::dotProduct(m_normal, a))
{
  // Intentionally Empty
}

inline void KPlane::set(float a, float b, float c, float d)
{
  float length = std::sqrt(a*a + b*b + c*c);
  m_normal = KVector3D(a, b, c);
  m_normal /= length;
  m_dTerm  = d / length;
}

inline float KPlane::dot(KVector3D const &point) const
{
  return KVector3D::dotProduct(m_normal, point) + m_dTerm;
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
