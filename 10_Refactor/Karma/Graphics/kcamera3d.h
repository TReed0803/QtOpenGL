#ifndef KCAMERA_H
#define KCAMERA_H

#include <KTransform3D>

class KCamera3D
{
public:

  // Constants
  static const QVector3D LocalForward;
  static const QVector3D LocalUp;
  static const QVector3D LocalRight;

  // Constructors
  KCamera3D();

  // Transform By (Add/Scale)
  void translate(const QVector3D &dt);
  void translate(float dx, float dy, float dz);
  void rotate(const QQuaternion &dr);
  void rotate(float angle, const QVector3D &axis);
  void rotate(float angle, float ax, float ay, float az);

  // Transform To (Setters)
  void setTranslation(const QVector3D &t);
  void setTranslation(float x, float y, float z);
  void setRotation(const QQuaternion &r);
  void setRotation(float angle, const QVector3D &axis);
  void setRotation(float angle, float ax, float ay, float az);

  // Accessors
  const QVector3D& translation() const;
  const QQuaternion& rotation() const;
  const QMatrix4x4& toMatrix();

  // Queries
  QVector3D forward() const;
  QVector3D up() const;
  QVector3D right() const;

private:
  bool m_dirty;
  QVector3D m_translation;
  QQuaternion m_rotation;
  QMatrix4x4 m_world;

#ifndef QT_NO_DATASTREAM
  friend QDataStream &operator<<(QDataStream &out, const KCamera3D &transform);
  friend QDataStream &operator>>(QDataStream &in, KCamera3D &transform);
#endif
};

Q_DECLARE_TYPEINFO(KCamera3D, Q_MOVABLE_TYPE);

// Constructors
inline KCamera3D::KCamera3D() : m_dirty(true) {}

// Transform By (Add/Scale)
inline void KCamera3D::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); }
inline void KCamera3D::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void KCamera3D::rotate(float angle, float ax, float ay,float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Transform To (Setters)
inline void KCamera3D::setTranslation(float x, float y, float z) { setTranslation(QVector3D(x, y, z)); }
inline void KCamera3D::setRotation(float angle, const QVector3D &axis) { setRotation(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void KCamera3D::setRotation(float angle, float ax, float ay, float az) { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Accessors
inline const QVector3D& KCamera3D::translation() const { return m_translation; }
inline const QQuaternion& KCamera3D::rotation() const { return m_rotation; }

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const KCamera3D &transform);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const KCamera3D &transform);
QDataStream &operator>>(QDataStream &in, KCamera3D &transform);
#endif

#endif // KCAMERA_H
