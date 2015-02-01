#include "kcamera3d.h"
#include <QDebug>

const KVector3D KCamera3D::LocalForward(0.0f, 0.0f, -1.0f);
const KVector3D KCamera3D::LocalUp(0.0f, 1.0f, 0.0f);
const KVector3D KCamera3D::LocalRight(1.0f, 0.0f, 0.0f);

// Transform By (Add/Scale)
void KCamera3D::translate(const KVector3D &dt)
{
  m_dirty = true;
  m_translation += dt;
}

void KCamera3D::rotate(const KQuaternion &dr)
{
  m_dirty = true;
  m_rotation = dr * m_rotation;
}

// Transform To (Setters)
void KCamera3D::setTranslation(const KVector3D &t)
{
  m_dirty = true;
  m_translation = t;
}

void KCamera3D::setRotation(const KQuaternion &r)
{
  m_dirty = true;
  m_rotation = r;
}

// Accessors
const KMatrix4x4 &KCamera3D::toMatrix()
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
KVector3D KCamera3D::forward() const
{
  return m_rotation.rotatedVector(LocalForward);
}

KVector3D KCamera3D::up() const
{
  return m_rotation.rotatedVector(LocalUp);
}

KVector3D KCamera3D::right() const
{
  return m_rotation.rotatedVector(LocalRight);
}

// Qt Streams
QDebug operator<<(QDebug dbg, const KCamera3D &transform)
{
  dbg << "Camera3D\n{\n";
  dbg << "Position: <" << transform.translation().x() << ", " << transform.translation().y() << ", " << transform.translation().z() << ">\n";
  dbg << "Rotation: <" << transform.rotation().x() << ", " << transform.rotation().y() << ", " << transform.rotation().z() << " | " << transform.rotation().scalar() << ">\n}";
  return dbg;
}

QDataStream &operator<<(QDataStream &out, const KCamera3D &transform)
{
  out << transform.m_translation;
  out << transform.m_rotation;
  return out;
}

QDataStream &operator>>(QDataStream &in, KCamera3D &transform)
{
  in >> transform.m_translation;
  in >> transform.m_rotation;
  transform.m_dirty = true;
  return in;
}

