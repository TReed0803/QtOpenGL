#ifndef OPENGLAREALIGHT_H
#define OPENGLAREALIGHT_H OpenGLAreaLight

#include <KVector3D>
#include <KTransform3D>

class OpenGLAreaLight
{
public:
  OpenGLAreaLight();

  // Translate
  void translate(float x, float y, float z);
  void translate(KVector3D const &trans);
  void setTranslation(float x, float y, float z);
  void setTranslation(KVector3D const &pos);
  KVector3D const &translation() const;

  // Rotation
  void rotate(float angle, float x, float y, float z);
  void rotate(float angle, const KVector3D &axis);
  void rotate(const KQuaternion &quat);
  void setRotation(float angle, float x, float y, float z);
  void setRotation(float angle, const KVector3D &axis);
  void setRotation(const KQuaternion &quat);
  const KQuaternion &rotation() const;
  void setDirection(const KVector3D &dir);
  void setDirection(const KVector3D &dir, const KVector3D &up);
  void setDirection(float x, float y, float z);
  KVector3D direction() const;

  // Smooth Attenuation
  void setRadius(float r);
  float radius() const;

  // Orientation
  KVector3D forward() const;
  KVector3D up() const;
  KVector3D right() const;

  void setTemperature(float temp);
  void setIntensity(float lm);
  float temperature() const;
  float intensity() const;
  KMatrix4x4 const& toMatrix() const;
  void setActive(bool a);
  bool active() const;

  KVector3D color() const;
private:
  bool  m_active;
  float m_radius;
  float m_temperature;
  float m_intensity;
  KTransform3D m_transform;
};

#endif // OPENGLAREALIGHT_H
