#include "openglarealight.h"
#include <KMath>

OpenGLAreaLight::OpenGLAreaLight() :
  m_active(true), m_radius(100.0f), m_temperature(2700.0f), m_intensity(1200.0f)
{
  // Intentionally Empty
}

void OpenGLAreaLight::translate(float x, float y, float z)
{
  m_transform.translate(x, y, z);
}

void OpenGLAreaLight::translate(const KVector3D &trans)
{
  m_transform.translate(trans);
}

void OpenGLAreaLight::setTranslation(float x, float y, float z)
{
  m_transform.setTranslation(KVector3D(x, y, z));
}

void OpenGLAreaLight::setTranslation(const KVector3D &trans)
{
  m_transform.setTranslation(trans);
}

void OpenGLAreaLight::setTemperature(float temp)
{
  m_temperature = temp;
}

void OpenGLAreaLight::setIntensity(float lm)
{
  m_intensity = lm;
}

const KVector3D &OpenGLAreaLight::translation() const
{
  return m_transform.translation();
}

void OpenGLAreaLight::rotate(float angle, float x, float y, float z)
{
  m_transform.rotate(angle, x, y, z);
}

void OpenGLAreaLight::rotate(float angle, const KVector3D &axis)
{
  m_transform.rotate(angle, axis);
}

void OpenGLAreaLight::rotate(const KQuaternion &quat)
{
  m_transform.rotate(quat);
}

void OpenGLAreaLight::setRotation(float angle, float x, float y, float z)
{
  m_transform.setRotation(angle, x, y, z);
}

void OpenGLAreaLight::setRotation(float angle, const KVector3D &axis)
{
  m_transform.setRotation(angle, axis);
}

void OpenGLAreaLight::setRotation(const KQuaternion &quat)
{
  m_transform.setRotation(quat);
}

const KQuaternion &OpenGLAreaLight::rotation() const
{
  return m_transform.rotation();
}

void OpenGLAreaLight::setDirection(const KVector3D &dir)
{
  m_transform.lookTowards(dir);
}

void OpenGLAreaLight::setDirection(const KVector3D &dir, const KVector3D &up)
{
  m_transform.lookTowards(dir, up);
}

void OpenGLAreaLight::setDirection(float x, float y, float z)
{
  m_transform.lookTowards(x, y, z);
}

KVector3D OpenGLAreaLight::direction() const
{
  return m_transform.forward();
}

void OpenGLAreaLight::setRadius(float r)
{
  m_radius = r;
}

float OpenGLAreaLight::radius() const
{
  return m_radius;
}

KVector3D OpenGLAreaLight::forward() const
{
  return m_transform.forward();
}

KVector3D OpenGLAreaLight::up() const
{
  return m_transform.up();
}

KVector3D OpenGLAreaLight::right() const
{
  return m_transform.right();
}

float OpenGLAreaLight::temperature() const
{
  return m_temperature;
}

float OpenGLAreaLight::intensity() const
{
  return m_intensity;
}

const KMatrix4x4 &OpenGLAreaLight::toMatrix() const
{
  return m_transform.toMatrix();
}

void OpenGLAreaLight::setActive(bool a)
{
  m_active = a;
}

bool OpenGLAreaLight::active() const
{
  return m_active;
}

KVector3D OpenGLAreaLight::color() const
{
  return Karma::k2rgb(m_temperature);
}
