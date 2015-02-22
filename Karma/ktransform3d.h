#ifndef KTRANSFORM3D_H
#define KTRANSFORM3D_H KTransform3D

#include <KMatrix4x4>
#include <KQuaternion>
#include <KVector3D>

class KTransform3D
{
public:

  // Constants
  static const KVector3D LocalForward;
  static const KVector3D LocalUp;
  static const KVector3D LocalRight;

  // Constructors
  KTransform3D();

  // Transform By (Add/Scale)
  void translate(const KVector3D &dt);
  void translate(float dx, float dy, float dz);
  void scale(const KVector3D &ds);
  void scale(float dx, float dy, float dz);
  void scale(float factor);
  void rotate(const KQuaternion &dr);
  void rotate(float angle, const KVector3D &axis);
  void rotate(float angle, float ax, float ay, float az);
  void grow(const KVector3D &ds);
  void grow(float dx, float dy, float dz);
  void grow(float factor);

  // Transform To (Setters)
  void setTranslation(const KVector3D &t);
  void setTranslation(float x, float y, float z);
  void setScale(const KVector3D &s);
  void setScale(float x, float y, float z);
  void setScale(float k);
  void setRotation(const KQuaternion &r);
  void setRotation(float angle, const KVector3D &axis);
  void setRotation(float angle, float ax, float ay, float az);

  // Accessors
  const KVector3D& translation() const;
  const KVector3D& scale() const;
  const KQuaternion& rotation() const;
  const KMatrix4x4& toMatrix();

  // Queries
  KVector3D forward() const;
  KVector3D up() const;
  KVector3D right() const;

private:
  bool m_dirty;
  KVector3D m_translation;
  KVector3D m_scale;
  KQuaternion m_rotation;
  KMatrix4x4 m_world;

#ifndef QT_NO_DATASTREAM
  friend QDataStream &operator<<(QDataStream &out, const KTransform3D &transform);
  friend QDataStream &operator>>(QDataStream &in, KTransform3D &transform);
#endif
};

Q_DECLARE_TYPEINFO(KTransform3D, Q_MOVABLE_TYPE);

inline KTransform3D::KTransform3D() : m_dirty(true), m_scale(1.0f, 1.0f, 1.0f) {}

// Transform By (Add/Scale)
inline void KTransform3D::translate(float dx, float dy,float dz) { translate(KVector3D(dx, dy, dz)); }
inline void KTransform3D::scale(float dx, float dy,float dz) { scale(KVector3D(dx, dy, dz)); }
inline void KTransform3D::scale(float factor) { scale(KVector3D(factor, factor, factor)); }
inline void KTransform3D::rotate(float angle, const KVector3D &axis) { rotate(KQuaternion::fromAxisAndAngle(axis, angle)); }
inline void KTransform3D::rotate(float angle, float ax, float ay,float az) { rotate(KQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }
inline void KTransform3D::grow(float dx, float dy, float dz) { grow(KVector3D(dx, dy, dz)); }
inline void KTransform3D::grow(float factor) { grow(KVector3D(factor, factor, factor)); }

// Transform To (Setters)
inline void KTransform3D::setTranslation(float x, float y, float z) { setTranslation(KVector3D(x, y, z)); }
inline void KTransform3D::setScale(float x, float y, float z) { setScale(KVector3D(x, y, z)); }
inline void KTransform3D::setScale(float k) { setScale(KVector3D(k, k, k)); }
inline void KTransform3D::setRotation(float angle, const KVector3D &axis) { setRotation(KQuaternion::fromAxisAndAngle(axis, angle)); }
inline void KTransform3D::setRotation(float angle, float ax, float ay, float az) { setRotation(KQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Accessors
inline const KVector3D& KTransform3D::translation() const { return m_translation; }
inline const KVector3D& KTransform3D::scale() const { return m_scale; }
inline const KQuaternion& KTransform3D::rotation() const { return m_rotation; }

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const KTransform3D &transform);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const KTransform3D &transform);
QDataStream &operator>>(QDataStream &in, KTransform3D &transform);
#endif

#endif // KTRANSFORM3D_H
