#ifndef QCAMERA_H
#define QCAMERA_H

#include "qtransform3d.h"

class QCamera3D
{
public:

  // Constants
  static const QVector3D LocalForward;
  static const QVector3D LocalUp;
  static const QVector3D LocalRight;

  // Constructors
  QCamera3D();

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
  friend QDataStream &operator<<(QDataStream &out, const QCamera3D &transform);
  friend QDataStream &operator>>(QDataStream &in, QCamera3D &transform);
#endif
};

Q_DECLARE_TYPEINFO(QCamera3D, Q_MOVABLE_TYPE);

// Constructors
inline QCamera3D::QCamera3D() : m_dirty(true) {}

// Transform By (Add/Scale)
inline void QCamera3D::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); }
inline void QCamera3D::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void QCamera3D::rotate(float angle, float ax, float ay,float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Transform To (Setters)
inline void QCamera3D::setTranslation(float x, float y, float z) { setTranslation(QVector3D(x, y, z)); }
inline void QCamera3D::setRotation(float angle, const QVector3D &axis) { setRotation(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void QCamera3D::setRotation(float angle, float ax, float ay, float az) { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Accessors
inline const QVector3D& QCamera3D::translation() const { return m_translation; }
inline const QQuaternion& QCamera3D::rotation() const { return m_rotation; }

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QCamera3D &transform);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const QCamera3D &transform);
QDataStream &operator>>(QDataStream &in, QCamera3D &transform);
#endif

#endif // QCAMERA_H
