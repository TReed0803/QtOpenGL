#ifndef OPENGLTRANSLATIONLIGHT_H
#define OPENGLTRANSLATIONLIGHT_H OpenGLTranslationLight

#include <OpenGLLight>
#include <KTransform3D>

class OpenGLTranslationLight : public OpenGLLight
{
public:

  // Translation
  void translate(float x, float y, float z);
  void translate(KVector3D const &trans);
  void setTranslation(float x, float y, float z);
  void setTranslation(KVector3D const &pos);
  KVector3D const &translation() const;
  void rotate(float angle, float x, float y, float z);
  void rotate(float angle, const KVector3D &axis);
  void rotate(const KQuaternion &quat);
  void setRotation(float angle, float x, float y, float z);
  void setRotation(float angle, const KVector3D &axis);
  void setRotation(const KQuaternion &quat);
  const KQuaternion &rotation() const;
  void setDirection(const KVector3D &dir);
  void setDirection(float x, float y, float z);
  KVector3D direction() const;
  const KMatrix4x4 &toMatrix() const;

protected:
  KTransform3D m_transform;
};


inline void OpenGLTranslationLight::translate(float x, float y, float z)
{
  m_transform.translate(x, y, z);
}

inline void OpenGLTranslationLight::translate(KVector3D const &trans)
{
  m_transform.translate(trans);
}

inline void OpenGLTranslationLight::setTranslation(float x, float y, float z)
{
  m_transform.setTranslation(x, y, z);
}

inline void OpenGLTranslationLight::setTranslation(KVector3D const &pos)
{
  m_transform.setTranslation(pos);
}

inline KVector3D const &OpenGLTranslationLight::translation() const
{
  return m_transform.translation();
}

inline void OpenGLTranslationLight::rotate(float angle, float x, float y, float z)
{
  m_transform.rotate(angle, x, y, z);
}

inline void OpenGLTranslationLight::rotate(float angle, const KVector3D &axis)
{
  m_transform.rotate(angle, axis);
}

inline void OpenGLTranslationLight::rotate(const KQuaternion &quat)
{
  m_transform.rotate(quat);
}

inline void OpenGLTranslationLight::setRotation(float angle, float x, float y, float z)
{
  m_transform.setRotation(angle, x, y, z);
}

inline void OpenGLTranslationLight::setRotation(float angle, const KVector3D &axis)
{
  m_transform.setRotation(angle, axis);
}

inline void OpenGLTranslationLight::setRotation(const KQuaternion &quat)
{
  m_transform.setRotation(quat);
}

inline const KQuaternion &OpenGLTranslationLight::rotation() const
{
  return m_transform.rotation();
}

inline void OpenGLTranslationLight::setDirection(const KVector3D &dir)
{
  m_transform.lookTowards(dir);
}

inline void OpenGLTranslationLight::setDirection(float x, float y, float z)
{
  m_transform.lookTowards(x, y, z);
}

inline KVector3D OpenGLTranslationLight::direction() const
{
  return m_transform.forward();
}

inline const KMatrix4x4 &OpenGLTranslationLight::toMatrix() const
{
  return m_transform.toMatrix();
}

#endif // OPENGLTRANSLATIONLIGHT_H
