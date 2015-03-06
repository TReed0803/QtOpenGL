#ifndef OPENGLSPOTLIGHT_H
#define OPENGLSPOTLIGHT_H OpenGLSpotLight

#include <KUniquePointer>
#include <KVector3D>

class KColor;
class KMatrix4x4;
class KQuaternion;

class OpenGLSpotLightPrivate;
class OpenGLSpotLight
{
public:
  OpenGLSpotLight();
  ~OpenGLSpotLight();

  // Static Scalar
  static float CalculateScalar(int segments);

  // Light Positioning
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
  const KMatrix4x4 &toMatrix();

  // Light Settings
  void setAttenuation(float c, float ex1, float ex2);
  void setAttenuation(KVector3D const &attn);
  KVector3D const &attenuation() const;
  void setDiffuse(float r, float g, float b);
  void setDiffuse(KColor const &rgb);
  KColor const &diffuse() const;
  void setSpecular(float r, float g, float b);
  void setSpecular(KColor const &rgb);
  KColor const &specular();
  void setInnerAngle(float angle);
  float innerAngle() const;
  void setOuterAngle(float angle);
  float outerAngle() const;
  void setDepth(float d);
  float depth() const;

private:
  KUniquePointer<OpenGLSpotLightPrivate> m_private;
};

#endif // OPENGLSPOTLIGHT_H
