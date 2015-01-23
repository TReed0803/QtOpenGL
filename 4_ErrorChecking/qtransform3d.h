#ifndef QTRANSFORM3D_H
#define QTRANSFORM3D_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class QTransform3D
{
public:

  // Constants
  static const QVector3D LocalForward;
  static const QVector3D LocalUp;
  static const QVector3D LocalRight;

  // Constructors
  QTransform3D();

  // Transform By (Add/Scale)
  void translate(const QVector3D &dt);
  void translate(float dx, float dy, float dz);
  void scale(const QVector3D &ds);
  void scale(float dx, float dy, float dz);
  void scale(float factor);
  void rotate(const QQuaternion &dr);
  void rotate(float angle, const QVector3D &axis);
  void rotate(float angle, float ax, float ay, float az);
  void grow(const QVector3D &ds);
  void grow(float dx, float dy, float dz);
  void grow(float factor);

  // Transform To (Setters)
  void setTranslation(const QVector3D &t);
  void setTranslation(float x, float y, float z);
  void setScale(const QVector3D &s);
  void setScale(float x, float y, float z);
  void setScale(float k);
  void setRotation(const QQuaternion &r);
  void setRotation(float angle, const QVector3D &axis);
  void setRotation(float angle, float ax, float ay, float az);

  // Accessors
  const QVector3D& translation() const;
  const QVector3D& scale() const;
  const QQuaternion& rotation() const;
  const QMatrix4x4& toMatrix();

  // Queries
  QVector3D forward() const;
  QVector3D up() const;
  QVector3D right() const;

private:
  bool m_dirty;
  QVector3D m_translation;
  QVector3D m_scale;
  QQuaternion m_rotation;
  QMatrix4x4 m_world;

#ifndef QT_NO_DATASTREAM
  friend QDataStream &operator<<(QDataStream &out, const QTransform3D &transform);
  friend QDataStream &operator>>(QDataStream &in, QTransform3D &transform);
#endif
};

Q_DECLARE_TYPEINFO(QTransform3D, Q_MOVABLE_TYPE);

inline QTransform3D::QTransform3D() : m_dirty(true), m_scale(1.0f, 1.0f, 1.0f) {}

// Transform By (Add/Scale)
inline void QTransform3D::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); }
inline void QTransform3D::scale(float dx, float dy,float dz) { scale(QVector3D(dx, dy, dz)); }
inline void QTransform3D::scale(float factor) { scale(QVector3D(factor, factor, factor)); }
inline void QTransform3D::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void QTransform3D::rotate(float angle, float ax, float ay,float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }
inline void QTransform3D::grow(float dx, float dy, float dz) { grow(QVector3D(dx, dy, dz)); }
inline void QTransform3D::grow(float factor) { grow(QVector3D(factor, factor, factor)); }

// Transform To (Setters)
inline void QTransform3D::setTranslation(float x, float y, float z) { setTranslation(QVector3D(x, y, z)); }
inline void QTransform3D::setScale(float x, float y, float z) { setScale(QVector3D(x, y, z)); }
inline void QTransform3D::setScale(float k) { setScale(QVector3D(k, k, k)); }
inline void QTransform3D::setRotation(float angle, const QVector3D &axis) { setRotation(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void QTransform3D::setRotation(float angle, float ax, float ay, float az) { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Accessors
inline const QVector3D& QTransform3D::translation() const { return m_translation; }
inline const QVector3D& QTransform3D::scale() const { return m_scale; }
inline const QQuaternion& QTransform3D::rotation() const { return m_rotation; }

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QTransform3D &transform);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const QTransform3D &transform);
QDataStream &operator>>(QDataStream &in, QTransform3D &transform);
#endif

#endif // QTRANSFORM3D_H
