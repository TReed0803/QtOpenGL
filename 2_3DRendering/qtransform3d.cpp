#include "qtransform3d.h"
#include <QDebug>

// Transform By (Add/Scale)
void QTransform3D::translate(const QVector3D &dt)
{
  m_dirty = true;
  m_translation += dt;
}

void QTransform3D::scale(const QVector3D &ds)
{
  m_dirty = true;
  m_scale *= ds;
}

void QTransform3D::rotate(const QQuaternion &dr)
{
  m_dirty = true;
  m_rotation *= dr;
}

void QTransform3D::grow(const QVector3D &ds)
{
  m_dirty = true;
  m_scale += ds;
}

// Transform To (Setters)
void QTransform3D::setTranslation(const QVector3D &t)
{
  m_dirty = true;
  m_translation = t;
}

void QTransform3D::setScale(const QVector3D &s)
{
  m_dirty = true;
  m_scale = s;
}

void QTransform3D::setRotation(const QQuaternion &r)
{
  m_dirty = true;
  m_rotation = r;
}

// Accessors
const QMatrix4x4 &QTransform3D::toMatrix()
{
  if (m_dirty)
  {
    m_dirty = false;
    m_world.setToIdentity();
    m_world.translate(m_translation);
    m_world.rotate(m_rotation);
    m_world.scale(m_scale);
  }
  return m_world;
}

// Qt Streams
QDebug operator<<(QDebug dbg, const QTransform3D &transform)
{
  dbg << "Transform3D\n{\n";
  dbg << "Position: <" << transform.translation().x() << ", " << transform.translation().y() << ", " << transform.translation().z() << ">\n";
  dbg << "Scale: <" << transform.scale().x() << ", " << transform.scale().y() << ", " << transform.scale().z() << ">\n";
  dbg << "Rotation: <" << transform.rotation().x() << ", " << transform.rotation().y() << ", " << transform.rotation().z() << " | " << transform.rotation().scalar() << ">\n}";
  return dbg;
}

QDataStream &operator<<(QDataStream &out, const QTransform3D &transform)
{
  out << transform.m_translation;
  out << transform.m_scale;
  out << transform.m_rotation;
  return out;
}

QDataStream &operator>>(QDataStream &in, QTransform3D &transform)
{
  in >> transform.m_translation;
  in >> transform.m_scale;
  in >> transform.m_rotation;
  transform.m_dirty = true;
  return in;
}
