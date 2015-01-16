#include "qcamera3d.h"
#include <QDebug>

QCamera3D::QCamera3D() : m_dirty(true)
{
}

// Transform By (Add/Scale)
void QCamera3D::translate(const QVector3D &dt)
{
  m_dirty = true;
  m_translation += dt;
}

void QCamera3D::rotate(const QQuaternion &dr)
{
  m_dirty = true;
  m_rotation = dr * m_rotation;
}

// Transform To (Setters)
void QCamera3D::setTranslation(const QVector3D &t)
{
  m_dirty = true;
  m_translation = t;
}

void QCamera3D::setRotation(const QQuaternion &r)
{
  m_dirty = true;
  m_rotation = r;
}

// Accessors
const QMatrix4x4 &QCamera3D::toMatrix()
{
  if (m_dirty)
  {
    m_dirty = false;
    m_world.setToIdentity();
    m_world.rotate(m_rotation.conjugate());
    m_world.translate(-m_translation);
  }
  return m_world;
}

// Queries
QVector3D QCamera3D::forward() const
{
  return m_rotation.rotatedVector(QVector3D(0.0f, 0.0f, -1.0f));
}

QVector3D QCamera3D::right() const
{
  return m_rotation.rotatedVector(QVector3D(1.0f, 0.0f, 0.0f));
}

QVector3D QCamera3D::up() const
{
  return m_rotation.rotatedVector(QVector3D(0.0f, 1.0f, 0.0f));
}

// Qt Streams
QDebug operator<<(QDebug dbg, const QCamera3D &transform)
{
  dbg << "Camera3D\n{\n";
  dbg << "Position: <" << transform.translation().x() << ", " << transform.translation().y() << ", " << transform.translation().z() << ">\n";
  dbg << "Rotation: <" << transform.rotation().x() << ", " << transform.rotation().y() << ", " << transform.rotation().z() << " | " << transform.rotation().scalar() << ">\n}";
  return dbg;
}

QDataStream &operator<<(QDataStream &out, const QCamera3D &transform)
{
  out << transform.m_translation;
  out << transform.m_rotation;
  return out;
}

QDataStream &operator>>(QDataStream &in, QCamera3D &transform)
{
  in >> transform.m_translation;
  in >> transform.m_rotation;
  transform.m_dirty = true;
  return in;
}

