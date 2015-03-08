#ifndef OPENGLDIRECTIONLIGHT_H
#define OPENGLDIRECTIONLIGHT_H OpenGLDirectionLight

#include <KVector3D>
#include <OpenGLLight>

class OpenGLDirectionLight : public OpenGLLight
{
public:
  OpenGLDirectionLight();

  //Light Settings
  void setDirection(float x, float y, float z);
  void setDirection(KVector3D const &dir);
  const KVector3D &direction() const;

private:
  KVector3D m_direction;
};

inline void OpenGLDirectionLight::setDirection(float x, float y, float z)
{
  setDirection(KVector3D(x, y ,z));
}

inline void OpenGLDirectionLight::setDirection(KVector3D const &dir)
{
  m_direction = dir.normalized();
}

inline const KVector3D &OpenGLDirectionLight::direction() const
{
  return m_direction;
}

#endif // OPENGLDIRECTIONLIGHT_H
