#include "ktransform3d.h"
#include <QDebug>
#include <KMath>

const KVector3D KTransform3D::LocalForward(0.0f, 0.0f, 1.0f);
const KVector3D KTransform3D::LocalUp(0.0f, 1.0f, 0.0f);
const KVector3D KTransform3D::LocalRight(1.0f, 0.0f, 0.0f);

// Transform By (Add/Scale)
void KTransform3D::translate(const KVector3D &dt)
{
  m_dirty = true;
  m_translation += dt;
}

void KTransform3D::scale(const KVector3D &ds)
{
  m_dirty = true;
  m_scale *= ds;
}

void KTransform3D::rotate(const KQuaternion &dr)
{
  m_dirty = true;
  m_rotation = dr * m_rotation;
}

void KTransform3D::grow(const KVector3D &ds)
{
  m_dirty = true;
  m_scale += ds;
}

// Transform To (Setters)
void KTransform3D::setTranslation(const KVector3D &t)
{
  m_dirty = true;
  m_translation = t;
}

void KTransform3D::setScale(const KVector3D &s)
{
  m_dirty = true;
  m_scale = s;
}

void KTransform3D::setRotation(const KQuaternion &r)
{
  m_dirty = true;
  m_rotation = r;
}

void KTransform3D::lookTowards(const KVector3D &dir)
{
  m_dirty = true;
  float dot = KVector3D::dotProduct(dir, KTransform3D::LocalForward);
  if (dot == 1.0f) return;

  float rotAngle = std::acos(dot);
  KVector3D rotAxis = KVector3D::crossProduct(KTransform3D::LocalForward, dir);
  rotAxis.normalize();
  setRotation(rotAngle * 180.0f / Karma::Pi, rotAxis);
}

void KTransform3D::setTranslationX(float x)
{
  m_dirty = true;
  m_translation.setX(x);
}

void KTransform3D::setTranslationY(float y)
{
  m_dirty = true;
  m_translation.setY(y);
}

void KTransform3D::setTranslationZ(float z)
{
  m_dirty = true;
  m_translation.setZ(z);
}

void KTransform3D::setScaleX(float x)
{
  m_dirty = true;
  m_scale.setX(x);
}

void KTransform3D::setScaleY(float y)
{
  m_dirty = true;
  m_scale.setY(y);
}

void KTransform3D::setScaleZ(float z)
{
  m_dirty = true;
  m_scale.setZ(z);
}

// Accessors
const KMatrix4x4 &KTransform3D::toMatrix()
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

// Queries
KVector3D KTransform3D::forward() const
{
  return m_rotation.rotatedVector(LocalForward);
}

KVector3D KTransform3D::up() const
{
  return m_rotation.rotatedVector(LocalUp);
}

KVector3D KTransform3D::right() const
{
  return m_rotation.rotatedVector(LocalRight);
}

// Qt Streams
QDebug operator<<(QDebug dbg, const KTransform3D &transform)
{
  dbg << "Transform3D\n{\n";
  dbg << "Position: <" << transform.translation().x() << ", " << transform.translation().y() << ", " << transform.translation().z() << ">\n";
  dbg << "Scale: <" << transform.scale().x() << ", " << transform.scale().y() << ", " << transform.scale().z() << ">\n";
  dbg << "Rotation: <" << transform.rotation().x() << ", " << transform.rotation().y() << ", " << transform.rotation().z() << " | " << transform.rotation().scalar() << ">\n}";
  return dbg;
}

QDataStream &operator<<(QDataStream &out, const KTransform3D &transform)
{
  out << transform.m_translation;
  out << transform.m_scale;
  out << transform.m_rotation;
  return out;
}

QDataStream &operator>>(QDataStream &in, KTransform3D &transform)
{
  in >> transform.m_translation;
  in >> transform.m_scale;
  in >> transform.m_rotation;
  transform.m_dirty = true;
  return in;
}
