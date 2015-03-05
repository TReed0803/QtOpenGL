#ifndef OPENGLDIRECTIONLIGHT_H
#define OPENGLDIRECTIONLIGHT_H OpenGLDirectionLight

#include <KUniquePointer>

class KColor;
class KVector3D;

class OpenGLDirectionLightPrivate;
class OpenGLDirectionLight
{
public:
  OpenGLDirectionLight();
  ~OpenGLDirectionLight();

  //Light Settings
  void setDirection(float x, float y, float z);
  void setDirection(KVector3D const &dir);
  KVector3D const &direction() const;
  void setDiffuse(float r, float g, float b);
  void setDiffuse(KColor const &rgb);
  KColor const &diffuse() const;
  void setSpecular(float r, float g, float b);
  void setSpecular(KColor const &rgb);
  KColor const &specular();

private:
  KUniquePointer<OpenGLDirectionLightPrivate> m_private;
};

#endif // OPENGLDIRECTIONLIGHT_H
