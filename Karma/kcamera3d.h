#ifndef KCAMERA3D_H
#define KCAMERA3D_H KCamera3D

#include <KMatrix4x4>
#include <KVector3D>
#include <KQuaternion>
#include <KFrustum>

class KCamera3D
{
public:

  // Constants
  static const KVector3D LocalForward;
  static const KVector3D LocalUp;
  static const KVector3D LocalRight;

  // Constructors
  KCamera3D();

  // Transform By (Add/Scale)
  void translate(const KVector3D &dt);
  void translate(float dx, float dy, float dz);
  void rotate(const KQuaternion &dr);
  void rotate(float angle, const KVector3D &axis);
  void rotate(float angle, float ax, float ay, float az);

  // Transform To (Setters)
  void setTranslation(const KVector3D &t);
  void setTranslation(float x, float y, float z);
  void setRotation(const KQuaternion &r);
  void setRotation(float angle, const KVector3D &axis);
  void setRotation(float angle, float ax, float ay, float az);
  void setProjection(KMatrix4x4 const &proj) const;

  // Accessors
  const KVector3D& translation() const;
  const KQuaternion& rotation() const;
  const KMatrix4x4& toMatrix() const;
  const KFrustum& frustum() const;
  bool dirty() const;

  // Queries
  KVector3D forward() const;
  KVector3D up() const;
  KVector3D right() const;
  float fieldOfView() const;

private:
  mutable bool m_dirty;
  float m_fovy;
  KVector3D m_translation;
  KQuaternion m_rotation;
  mutable KMatrix4x4 m_world;
  mutable KMatrix4x4 m_projection;
  mutable KFrustum m_frustum;

  void clean() const;

#ifndef QT_NO_DATASTREAM
  friend QDataStream &operator<<(QDataStream &out, const KCamera3D &transform);
  friend QDataStream &operator>>(QDataStream &in, KCamera3D &transform);
#endif
};

Q_DECLARE_TYPEINFO(KCamera3D, Q_MOVABLE_TYPE);

// Constructors
inline KCamera3D::KCamera3D() : m_dirty(true), m_fovy(45.0f) {}

// Transform By (Add/Scale)
inline void KCamera3D::translate(float dx, float dy,float dz) { translate(KVector3D(dx, dy, dz)); }
inline void KCamera3D::rotate(float angle, const KVector3D &axis) { rotate(KQuaternion::fromAxisAndAngle(axis, angle)); }
inline void KCamera3D::rotate(float angle, float ax, float ay,float az) { rotate(KQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Transform To (Setters)
inline void KCamera3D::setTranslation(float x, float y, float z) { setTranslation(KVector3D(x, y, z)); }
inline void KCamera3D::setRotation(float angle, const KVector3D &axis) { setRotation(KQuaternion::fromAxisAndAngle(axis, angle)); }
inline void KCamera3D::setRotation(float angle, float ax, float ay, float az) { setRotation(KQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Accessors
inline const KVector3D& KCamera3D::translation() const { return m_translation; }
inline const KQuaternion& KCamera3D::rotation() const { return m_rotation; }

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const KCamera3D &transform);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const KCamera3D &transform);
QDataStream &operator>>(QDataStream &in, KCamera3D &transform);
#endif

#endif // KCAMERA3D_H
